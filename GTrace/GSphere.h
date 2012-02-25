#pragma once

#include <Eigen/core>
#include "GRay.h"

namespace gtrace 
{

	class GSphere 
	{
		public:
			GSphere();
			GSphere(Eigen::Vector3f position, float radius);

			bool Intersect(const GRay& ray, float& distance, Eigen::Vector3f& normal) const;

		private:
			Eigen::Vector3f m_position;
			float m_radius;

	};
}
