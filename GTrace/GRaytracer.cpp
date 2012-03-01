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

	void GRaytracer::Render(std::string file_name)
	{
		using namespace geometry;
		using namespace material;
		Vector3f lightdir(-Vector3f::UnitY());

		unsigned char color[3];
		for(unsigned int x = 0; x < m_camera->GetImageWidth(); ++x)
		{
			for(unsigned int y =0; y < m_camera->GetImageHeight(); ++y)
			{
				GRay ray = m_camera->GetRayForPosition(x, y);
				GHit hit;

				if (TraceRay(ray, hit))
				{
			
					Vector3f col(Vector3f::Zero());
					GMaterial* mat = const_cast<GMaterial*>(hit.obj->GetMaterial()); // TODO: const correctness...

					GRay shadow_ray(hit.position + hit.normal * 0.00001f, -lightdir);
					GHit hit2;
					float diffuse = 0.0f;
					float spec = 0.0f;
					float ambient_occlusion = 1.0f;
					if (TraceShadowRay(shadow_ray, hit2, hit.obj)) ambient_occlusion = clamp(hit2.distance / 0.2f);
					else
					{
						diffuse = clamp(-lightdir.dot(hit.normal));
						Vector3f refl_vec = ray.m_direction + hit.normal * ray.m_direction.dot(hit.normal) * 2.0f;	
						spec =  pow(clamp(lightdir.dot(refl_vec)), mat->m_shininess(hit));

						col += diffuse * mat->m_diffuse(hit) + spec * mat->m_specular(hit);
					}
					col += mat->m_ambient(hit) * ambient_occlusion;
					color[0] = col[0] > 1.0f ? 255 : col[0] * 255;
					color[1] = col[1] > 1.0f ? 255 : col[1] * 255;
					color[2] = col[2] > 1.0f ? 255 : col[2] * 255;

					m_render_buffer->draw_point(x, y, color);
					
				}

			}
		}
		m_render_buffer->save(file_name.c_str());
	}
			
}
