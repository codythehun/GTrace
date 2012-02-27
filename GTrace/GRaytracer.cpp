#include "stdafx.h"
#include "GRaytracer.h"
#include "GCamera.h"
#include "GGeometry.h"

#include <Eigen/core>
#include <float.h>

using namespace Eigen;
using namespace std;
using namespace cimg_library;

namespace gtrace
{
	using namespace geometry;

	GRaytracer::GRaytracer():m_camera(new GCamera()), m_render_buffer(new CImg<unsigned char>(m_camera->GetImageWidth(), m_camera->GetImageHeight(), 1, 3)) {}

	GRaytracer:: ~GRaytracer()
	{
		if (m_camera) delete m_camera;
		if (m_render_buffer) delete m_render_buffer;
	}

	void GRaytracer::SetCamera(GCamera* camera)
	{
		if ( camera == m_camera) return;
		delete m_camera;
		m_camera = camera;
		delete m_render_buffer;
		m_render_buffer = new CImg<unsigned char> (m_camera->GetImageWidth(), m_camera->GetImageHeight(), 1, 3);
	}
	void GRaytracer::AddObject(GBody* object)
	{
		m_objects.push_back(object);
	}

	bool GRaytracer::TraceRay(const GRay& ray, float& distance, Vector3f& normal, GBody* &obj)
	{
		float min_distance = FLT_MAX;
		Vector3f min_normal;
		GBody* hit = 0;

		for(auto obj_it = m_objects.begin(); obj_it != m_objects.end(); ++obj_it)
		{
			if ((*obj_it)->Intersect(ray, distance, normal) && distance < min_distance)
			{
				min_distance = distance;
				min_normal = normal;
				hit = *obj_it;
			}
		}
		if (hit)
		{
			distance = min_distance;
			normal = min_normal;
			obj = hit;
			return true;
		}
		return false;
	}

	void GRaytracer::Render(std::string file_name)
	{
		Vector3f lightdir(-Vector3f::UnitY());

		unsigned char color[3];
		for(unsigned int x = 0; x < m_camera->GetImageWidth(); ++x)
		{
			for(unsigned int y =0; y < m_camera->GetImageHeight(); ++y)
			{
				geometry::GRay ray = m_camera->GetRayForPosition(x, y);
				float distance;
				Vector3f normal;
				GBody* hit;

				if (TraceRay(ray, distance, normal, hit))
				{
			
					Vector3f col(hit->m_material.m_ambient);

					GRay shadow_ray(ray.m_origin + ray.m_direction * distance + normal * 0.00001f, -lightdir);
					float distance2;
					Vector3f normal2;
					GBody* hit2;
					float diffuse = 0.0f;
					float spec = 0.0f;
					if (!TraceRay(shadow_ray, distance2, normal2, hit2))
					{
						diffuse = clamp(-lightdir.dot(normal));
						Vector3f refl_vec = ray.m_direction + normal * ray.m_direction.dot(normal) * 2.0f;
					
						spec =  pow(clamp(lightdir.dot(refl_vec)), hit->m_material.m_shininess);
					}
					col += diffuse * hit->m_material.m_diffuse + spec * hit->m_material.m_specular;
					
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
