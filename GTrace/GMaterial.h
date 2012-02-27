#pragma once

#include <Eigen/core>
#include "GGeometry.h"
#include <Eigen\core>

namespace gtrace 
{
namespace material
{
	class GMaterial
	{
	public:
		Eigen::Vector3f m_diffuse;
		Eigen::Vector3f m_ambient;
		Eigen::Vector3f m_specular;
		float m_transparency;
		float m_reflection;
		float m_refraction_index;
		float m_freshnel_coeff;
		float m_shininess;
	};
}
}