#pragma once

#include <Eigen/core>
#include "GGeometry.h"

namespace gtrace 
{
namespace geometry
{

	class GSphere : public GBody
	{
		public:
			GSphere();
			GSphere(Eigen::Vector3f position, float radius);

			bool Intersect(const GRay& ray, float& distance, Eigen::Vector3f& normal) const;
			bool Intersect(const GRay& ray, float& distance) const;
			bool Intersect(const GRay& ray) const;

		private:
			Eigen::Vector3f m_position;
			float m_radius;

	};
}
}