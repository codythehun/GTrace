#pragma once

#include <Eigen/core>
#include <Eigen/geometry>
#include "GMaterial.h"

namespace gtrace
{
namespace geometry 
{
	float Deg2Rad(float degree);
	float Rad2Deg(float radian);
	float clamp(float value, float min=0.0f, float max=1.0f);
	
	class GRay
	{
		public:
			GRay();
			GRay(Eigen::Vector3f direction);
			GRay(Eigen::Vector3f origin, Eigen::Vector3f direction);

			Eigen::Vector3f m_direction;
			Eigen::Vector3f m_origin;

	};

	class GBody
	{
		public:
			material::GMaterial m_material; // gross...

			virtual bool Intersect(const GRay& ray, float& distance, Eigen::Vector3f& normal) const = 0;
			virtual bool Intersect(const GRay& ray, float& distance) const = 0;
			virtual bool Intersect(const GRay& ray) const = 0;
			// future ideas:

			// -- Materials:
			// should also be able to calculate texture coordinates for an intersection
			// could return many texture coordinates, according to how many texture coordinate channels it has
			// texture coordinate channels could have a texture coordinate unwrapper assigned to them, which calculates the coordinates
			// based on the tex coordinates, the material of te object can be calculated
			// material has different properties, which should have resolvers, which could return a constant value, or lookup in a texture based on a texture coordinate channel

			// -- Lighting:
			// Should also be able to return a random point on the surface, which will be good for monte carlo simulations (shadow rays, area lights etc)


	};

}
}