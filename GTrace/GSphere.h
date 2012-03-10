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
			~GSphere() {}
			GSphere(Eigen::Vector3f position, float radius);

			bool Intersect(const GRay& ray, GHit& hit) const;
			
			 Eigen::Vector3f GetSamplePoint(int n, int count)const;

		private:
			float m_radius;

	};
}
}