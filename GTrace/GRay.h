#pragma once

#include <Eigen/core>


namespace gtrace 
{

	class GRay
	{
		public:
			GRay();
			GRay(Eigen::Vector3f direction);
			GRay(Eigen::Vector3f origin, Eigen::Vector3f direction);

			Eigen::Vector3f m_direction;
			Eigen::Vector3f m_origin;

	};
}
