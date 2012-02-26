#include "stdafx.h"
#include "GCamera.h"
#include "GGeometry.h"

#include <Eigen/core>
#include <Eigen/geometry>


using namespace Eigen;

namespace gtrace
{

	GCamera::GCamera(unsigned int img_width, unsigned int img_height, float fov) : m_hfov(fov), m_pitch(0), m_yaw(0), m_roll(0)
	{
		SetImageDimensions(img_width, img_height);
		CalculateFrustum();
		m_position = Vector3f::Zero();
	}

	void GCamera::SetImageDimensions(unsigned int img_width, unsigned int img_height)
	{
		m_img_width = img_width;
		m_img_height = img_height;
		m_aspect_ratio = (float)m_img_width / m_img_height;
		m_vfov = m_hfov / m_aspect_ratio;
	}

	void GCamera::SetFieldOfView(float fov)
	{
		m_hfov = fov;
		m_vfov = m_hfov / m_aspect_ratio;
		CalculateFrustum();
	}

	void GCamera::SetPosition(Eigen::Vector3f position)
	{
		m_position = position;
	}

	void GCamera::SetOrientation(float pitch, float yaw, float roll)
	{
		m_pitch = pitch;
		m_yaw = yaw;
		m_roll = roll;
		CalculateFrustum();
	}


	geometry::GRay GCamera::GetRayForPosition(unsigned int x, unsigned int y) const
	{
		Vector3f direction = m_frustum[0] + m_horizontal_step * x + m_vertical_step * y;
		direction.normalize();
		geometry::GRay res(m_position, direction);
		return res;
	}

	void GCamera::CalculateFrustum()
	{
		Matrix3f m;
		float x = geometry::Deg2Rad(m_vfov/2.0f);
		float y = geometry::Deg2Rad(m_hfov/2.0f);
		// calculating rotation matrix based on pitch yaw roll
		Matrix3f rot;
		rot =	AngleAxisf(geometry::Deg2Rad(m_roll), Vector3f::UnitZ()) *
				AngleAxisf(geometry::Deg2Rad(m_pitch), Vector3f::UnitX()) *
				AngleAxisf(geometry::Deg2Rad(m_yaw), Vector3f::UnitY());

		// calculating frustum vecs
		m = AngleAxisf(x, Vector3f::UnitX())*  
			AngleAxisf(y, Vector3f::UnitY()) * rot;
		m_frustum[0] = Vector3f::UnitZ().transpose() * m;

		m = AngleAxisf(x, Vector3f::UnitX())*  
			AngleAxisf(-y, Vector3f::UnitY()) * rot;
		m_frustum[1] = Vector3f::UnitZ().transpose() * m;

		m = AngleAxisf(-x, Vector3f::UnitX())*  
			AngleAxisf(y, Vector3f::UnitY()) * rot;
		m_frustum[2] = Vector3f::UnitZ().transpose() * m;

		m = AngleAxisf(-x, Vector3f::UnitX())*  
			AngleAxisf(-y, Vector3f::UnitY()) * rot;
		m_frustum[3] = Vector3f::UnitZ().transpose() * m;

		CalculateStepVectors();
		
	}

	void GCamera::CalculateStepVectors()
	{
		m_horizontal_step = (m_frustum[1] - m_frustum[0]) / (float) m_img_width;
		m_vertical_step = (m_frustum[2] - m_frustum[0]) / (float) m_img_height;
	}

}