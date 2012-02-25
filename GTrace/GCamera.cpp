#include "stdafx.h"
#include "GCamera.h"
#include "Eigen/core"
#include "Eigen/geometry"

const float ANG2RAD = 2 * M_PI / 360.0;
const float RAD2ANG = 360 / (2 * M_PI);

using namespace Eigen;

namespace gtrace
{

	GCamera::GCamera(unsigned int img_width, unsigned int img_height, float fov) : m_hfov(fov), m_pitch(0), m_yaw(0), m_roll(0)
	{
		SetImageDimensions(img_width, img_height);
		CalculateFrustum();
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

	void GCamera::SetOrientation(float pitch, float yaw, float roll)
	{
		m_pitch = pitch;
		m_yaw = yaw;
		m_roll = roll;
		CalculateFrustum();
	}


	Vector3f GCamera::GetRayForPosition(unsigned int x, unsigned int y) const
	{
		float cx = (float)x / m_img_width;
		float cy = (float)y / m_img_height;

		Vector3f v1 = m_frustum[1] * cx + m_frustum[0] * (1.0f - cx);
		Vector3f v2 = m_frustum[3] * cx + m_frustum[2] * (1.0f - cx);
		Vector3f res = v2 * cy + v1 * (1.0f - cy);
		res.normalize();
		return res;

	}

	void GCamera::CalculateFrustum()
	{
		Matrix3f m;
		float x = m_vfov/2.0f * ANG2RAD;
		float y = m_hfov/2.0f * ANG2RAD;
		// calculating rotation matrix based on pitch yaw roll
		Matrix3f rot;
		rot =	AngleAxisf(m_roll * ANG2RAD, Vector3f::UnitZ()) *
				AngleAxisf(m_pitch * ANG2RAD, Vector3f::UnitX()) *
				AngleAxisf(m_yaw * ANG2RAD, Vector3f::UnitY());

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
		
	}

}