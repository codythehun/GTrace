#pragma once

#include <Eigen/core>
#include "GGeometry.h"

namespace gtrace 
{

	class GCamera
	{
		public:

			GCamera(unsigned int img_width = 640, unsigned int img_height = 480, float fov = 60);

			void SetImageDimensions(unsigned int img_width, unsigned int img_height);
			void SetFieldOfView(float fov);
			//void SetOrientation(Eigen::Vector3f direction, float roll = 0.0f);
			void SetOrientation(float pitch, float yaw, float roll);
			void SetPosition(Eigen::Vector3f position);

			geometry::GRay GetRayForPosition(unsigned int x, unsigned int y) const;

			unsigned int GetImageWidth() const { return m_img_width; };
			unsigned int GetImageHeight() const { return m_img_height; }; 
		protected:
			void CalculateFrustum();
			void CalculateStepVectors();

			//viewport
			unsigned int m_img_width;
			unsigned int m_img_height;
			float m_hfov, m_vfov, m_aspect_ratio;

			// orinetation
			float m_pitch, m_yaw, m_roll;
			Eigen::Vector3f m_position;

			// frustum
			Eigen::Vector3f m_frustum[4]; // topleft, topright, bottomleft, bottomright
			Eigen::Vector3f m_horizontal_step; // stepping accross screen horizontally
			Eigen::Vector3f m_vertical_step; // stepping accross screen vertically

			

			
	};

}