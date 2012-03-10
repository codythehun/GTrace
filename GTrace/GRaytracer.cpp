#include "stdafx.h"
#include "GRaytracer.h"
#include "GCamera.h"
#include "GGeometry.h"
#include "GMaterial.h"

#include <Eigen/core>
#include <float.h>
#include <algorithm>

using namespace Eigen;
using namespace std;
using namespace cimg_library;

const float EPSILON = 0.0001f;
namespace gtrace
{


	GRaytracer::GRaytracer():m_camera(new GCamera()), m_render_buffer(new CImg<unsigned char>(m_camera->GetImageWidth(), m_camera->GetImageHeight(), 1, 3)) {}

	GRaytracer:: ~GRaytracer()
	{
		if (m_camera) delete m_camera;
		if (m_render_buffer) delete m_render_buffer;
		
		for_each(m_objects.begin(), m_objects.end(), [](geometry::GBody* obj) { delete obj; });
	}

	void GRaytracer::SetCamera(GCamera* camera)
	{
		if ( camera == m_camera) return;
		delete m_camera;
		m_camera = camera;
		delete m_render_buffer;
		m_render_buffer = new CImg<unsigned char> (m_camera->GetImageWidth(), m_camera->GetImageHeight(), 1, 3);
	}
	void GRaytracer::AddObject(geometry::GBody* object)
	{
		m_objects.push_back(object);
	}

	bool GRaytracer::TraceShadowRay(const geometry::GRay& ray, geometry::GHit& hit, const geometry::GBody* ignore)
	{
		using namespace geometry;
		float min_distance = FLT_MAX;
		bool found = false;
		GHit res;
		res.fields = GHit::DISTANCE;

		for(auto obj_it = m_objects.begin(); obj_it != m_objects.end(); ++obj_it)
		{
			if (*obj_it != ignore && (*obj_it)->Intersect(ray, res) && res.distance < min_distance)
			{
				min_distance = res.distance;
				hit = res;
				found = true;
			}
		}
		return found;
	}

	bool GRaytracer::TraceRay(const geometry::GRay& ray, geometry::GHit& hit)
	{
		using namespace geometry;
		float min_distance = FLT_MAX;
		bool found = false;
		GHit res;
		res.fields = GHit::ALL;
		

		for(auto obj_it = m_objects.begin(); obj_it != m_objects.end(); ++obj_it)
		{
			if ((*obj_it)->Intersect(ray, res) && res.distance < min_distance)
			{
				min_distance = res.distance;
				hit = res;
				found = true;
			}
		}
		return found;
	}

	Vector3f GRaytracer::TraceRay(const geometry::GRay& ray, int depth)
	{
		using namespace geometry;
		using namespace material;
		Vector3f lightdir(-Vector3f::UnitY());
		Vector3f col(Vector3f::Zero());
		GHit hit;

		if (TraceRay(ray, hit))
		{
			GMaterial* mat = const_cast<GMaterial*>(hit.obj->GetMaterial()); // TODO: const correctness...
			Vector3f refl_vec, refr_vec;

			if(m_options.Applicable(depth, Options::REFRACTION) && mat->m_opacity(hit) < 1.0f)
			{
				float dot1 = hit.normal.dot(-ray.m_direction);
				float refr_index = mat->m_refraction_index(hit);
				if(dot1 > 0.0f) refr_index = 1.0f / refr_index;
				
				float dot2 = sqrt(1.0f - refr_index * refr_index * (1.0f - dot1 * dot1));
				if(dot1 > 0.0f) // ray enterint object
				{
					refr_vec = refr_index * ray.m_direction - (refr_index * dot1 - dot2) * hit.normal;
					refr_vec.normalize();
					GRay refr_ray(hit.position - hit.normal * EPSILON, refr_vec);
					col += (1.0f - mat->m_opacity(hit))*TraceRay(refr_ray, depth); // no depth counting for passing through ray
				}
				else // ray leaving object
				{
					dot1 = ray.m_direction.dot(-hit.normal);
					dot2 = sqrt(1.0f - refr_index * refr_index * (1.0f - dot1 * dot1));
			
					refr_vec = refr_index * ray.m_direction - (refr_index * dot1 - dot2) * hit.normal;
					refr_vec.normalize();
					GRay refr_ray(hit.position + hit.normal * EPSILON, refr_vec);
					return TraceRay(refr_ray, depth+1); // inside of object, no other terms computed
				}
			}

			if(m_options.Applicable(depth, Options::REFLECTION) || 
				m_options.Applicable(depth, Options::SPECULAR))
				refl_vec = ray.m_direction - hit.normal * ray.m_direction.dot(hit.normal) * 2.0f;	

			for(auto light_it = m_lights.begin(); light_it != m_lights.end(); ++light_it)
			{
				int samples = 1;
				float weight = 1.0f/1.0f;
				for(int i=0; i<samples; ++i)
				{
					//Vector3f light_vec = (*light_it)->GetRandomPoint() - hit.position;
					Vector3f light_vec = (*light_it)->GetPosition() - hit.position;
					
					light_vec.normalize();
					bool shadow = false;

					if(m_options.Applicable(depth, Options::SHADOW))
					{
						GRay shadow_ray(hit.position + hit.normal * EPSILON, light_vec);
						GHit sh_hit;
						if (TraceShadowRay(shadow_ray, sh_hit, hit.obj)) shadow = true;
					}
					
					if(!shadow)
					{
						if(m_options.Applicable(depth, Options::SPECULAR))
						{
							col += mat->m_specular(hit) * pow(clamp(light_vec.dot(refl_vec)), mat->m_shininess(hit)) * mat->m_opacity(hit) * weight;
						}
						col +=  mat->m_diffuse(hit) * clamp(light_vec.dot(hit.normal)) * mat->m_opacity(hit)  * weight ;

					}
				}
				
			}

	
			if(m_options.Applicable(depth, Options::REFLECTION) && mat->m_reflection(hit) > 0.0f)
			{
				GRay refl_ray(hit.position + hit.normal * EPSILON, refl_vec);
				col += TraceRay(refl_ray, depth + 1) * mat->m_reflection(hit) ;//* pow(1.0f + ray.m_direction.dot(hit.normal), mat->m_freshnel_coeff(hit));
			}
			col += mat->m_ambient(hit) * mat->m_opacity(hit) ;
		}
		return col;
	}

	void GRaytracer::Render(std::string file_name)
	{
		using namespace geometry;
		using namespace material;
		Vector3f lightdir(-Vector3f::UnitY());
		Vector3f col;

		unsigned char color[3];
		for(unsigned int x = 0; x < m_camera->GetImageWidth(); ++x)
		{
			for(unsigned int y =0; y < m_camera->GetImageHeight(); ++y)
			{
				GRay ray = m_camera->GetRayForPosition(x, y);
				col = TraceRay(ray);
				color[0] = col[0] > 1.0f ? 255 : col[0] * 255;
				color[1] = col[1] > 1.0f ? 255 : col[1] * 255;
				color[2] = col[2] > 1.0f ? 255 : col[2] * 255;

				m_render_buffer->draw_point(x, y, color);
					
			}

		
		}
		m_render_buffer->save(file_name.c_str());
	}
			
	void GRaytracer::AddLight(geometry::GBody* light)
	{
		m_lights.push_back(light);
	}
			
}
