
#include "stdafx.h"
#include "GGeometry.h"
#include "GPlane.h"
#include <Eigen/core>

using namespace Eigen;

namespace gtrace
{
namespace geometry
{

	GPlane::GPlane(): m_dist(0), m_normal(Vector3f::UnitY()) {}

	GPlane::GPlane(Vector3f normal, float dist): m_normal(normal), m_dist(dist)
	{
		m_normal.normalize();
	}
	
	GPlane::GPlane(Vector3f position, Vector3f normal):GBody(position), m_normal(normal)
	{
		m_dist = m_normal.dot(m_position);
	}

	bool GPlane::Intersect(const GRay& ray, GHit& hit) const
	{
		float product = m_normal.dot(ray.m_direction);
		if (product == 0.0f) return false; // plane and ray are parallel
		float d = ray.m_origin.dot(m_normal); // distance of ray origin from plane moved to the origo
		float dist = (m_dist - d) / product; // distance from ray origin to plane
		if( dist < 0.0f) return false;

		hit.obj = static_cast<const GBody*>(this);
		if(hit.fields & GHit::DISTANCE) hit.distance = dist;
		if(hit.fields & GHit::NORMAL) hit.normal = m_normal;
		if(hit.fields & GHit::POSITION) 
		{
			hit.position = ray.m_origin + ray.m_direction * dist;
			if(hit.fields & GHit::TEXCOORDS)
			{
				// TODO: extract code to UnWrapper classes
				Vector3f axis_x(Vector3f::UnitX());
				Vector3f axis_z = m_normal.cross(axis_x);
				axis_z.normalize();
				axis_x = axis_z.cross(m_normal);
				axis_x.normalize();
				Matrix3f tangent_space;
				tangent_space << axis_x , m_normal , axis_z;
				RowVector3f uv = hit.position.transpose() * tangent_space;
				hit.u = uv.x();
				hit.v = uv.z();
			}


		}

		return true;
	}

	Vector3f GPlane::GetSamplePoint(int n, int count)const
	{
		return Vector3f(); // dummy implementation for now
	}
	

}
}