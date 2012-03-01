
#include "stdafx.h"
#include "GGeometry.h"
#include "GSphere.h"
#include <Eigen/core>

using namespace Eigen;

namespace gtrace
{
namespace geometry
{

	GSphere::GSphere(): m_radius(1), m_position(Vector3f::Zero()) {}

	GSphere::GSphere(Vector3f position, float radius): m_position(position), m_radius(radius) {}

	bool GSphere::Intersect(const GRay& ray, GHit& hit) const
	{
		float x1, x2; // x is the unknown: distance to travel along ray to hit sphere
		// equation: ||ray.m_origin + x * ray.m_direction - m_position || = m_radius
		
		Vector3f R = ray.m_origin - m_position; // put sphere into origo, to remove m_position from equation
		
		// restructure equation to get standard quadratic form: ax^2 + bx + c = 0
		// a = ray.m_direction.dot(ray.m_direction) = 1, because it is unit length (prerequisite of rays) 
		float b = R.dot(ray.m_direction) * 2.0f; 
		float c = R.dot(R) - m_radius * m_radius;
		float discr = b * b - 4 * c;
		if ( discr < 0.0f) return false; // discriminant is negative, no solution
		discr = sqrt(discr);
		x2 = -b + discr;
		x1 = -b - discr;
		if( x2 < 0.0f ) return false; // sphere is behind ray origin

		hit.obj = static_cast<const GBody*>(this);
		if(hit.fields & (GHit::DISTANCE | GHit::POSITION | GHit::NORMAL))
		{
			if ( x1 >= 0.0f) hit.distance = x1 / 2.0f;
			else hit.distance = x2 / 2.0f; // should I negate normal then ??
		}
		if(hit.fields & GHit::POSITION) hit.position = ray.m_origin + ray.m_direction * hit.distance;
		if(hit.fields & GHit::NORMAL) hit.normal = (R + ray.m_direction * hit.distance) / m_radius;
		
		if(hit.fields & GHit::TEXCOORDS){
		//TODO implement
		}

		
		return true;
	}


}
}