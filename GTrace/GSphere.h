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

			bool Intersect(const GRay& ray, GHit& hit) const;

		private:
			Eigen::Vector3f m_position;
			float m_radius;

	};
}
}