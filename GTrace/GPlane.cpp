
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
	
	bool GPlane::Intersect(const GRay& ray, GHit& hit) const
	{
		float product = m_normal.dot(ray.m_direction);
		if (product == 0.0f) return false; // plane and ray are parallel
		float d = ray.m_origin.dot(m_normal); // distance of ray origin from plane moved to the origo
		float dist = (m_dist - d) / product; // distance from ray origin to plane
		if( dist < 0.0f) return false;

		if(hit.fields & GHit::DISTANCE) hit.distance = dist;
		if(hit.fields & GHit::NORMAL) hit.normal = m_normal;
		if(hit.fields & GHit::POSITION) hit.position = ray.m_origin + ray.m_direction * dist;
		if(hit.fields & GHit::TEXCOORDS){
		//TODO implement
		}

		return true;
	}

	

}
}