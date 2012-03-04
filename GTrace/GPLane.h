#pragma once

#include <Eigen/core>
#include "GGeometry.h"

namespace gtrace 
{
namespace geometry
{

	class GPlane : public GBody
	{
		public:
			GPlane();
			GPlane(Eigen::Vector3f point, Eigen::Vector3f normal);
			GPlane(Eigen::Vector3f normal, float dist);

			bool Intersect(const GRay& ray, GHit& hit) const;
			~GPlane() {}

			Eigen::Vector3f GetRandomPoint()const;

		private:
			Eigen::Vector3f m_normal;
			float m_dist;

	};
}
}
