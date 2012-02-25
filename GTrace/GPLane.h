#pragma once

#include <Eigen/core>
#include "GRay.h"

namespace gtrace 
{

	class GPlane 
	{
		public:
			GPlane();
			GPlane(Eigen::Vector3f point, Eigen::Vector3f normal);
			GPlane(Eigen::Vector3f normal, float dist);

			bool Intersect(const GRay& ray, float& distance, Eigen::Vector3f& normal) const;

		private:
			Eigen::Vector3f m_normal;
			float m_dist;

	};
}
