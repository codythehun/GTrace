#pragma once

#include <Eigen/core>
#include <Eigen/geometry>

namespace gtrace
{
namespace geometry 
{
	float Deg2Rad(float degree);
	float Rad2Deg(float radian);
	
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
			virtual bool Intersect(const GRay& ray, float& distance, Eigen::Vector3f& normal) const = 0;
			virtual bool Intersect(const GRay& ray, float& distance) const = 0;
			virtual bool Intersect(const GRay& ray) const = 0;
	};

}
}