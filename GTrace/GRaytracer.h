#pragma once

#include "GCamera.h"
#include "GGeometry.h"
#include <CImg/CImg.h>
#include <list>
#include <string>
#include <Eigen\core>

namespace gtrace
{
	class GRaytracer
	{
	public:
		GRaytracer();
		~GRaytracer();

		void SetCamera(GCamera* camera);
		void AddObject(geometry::GBody* object);

		void Render(std::string file_name);
	protected:
		bool TraceRay(const geometry::GRay& ray, geometry::GHit& hit);
		bool TraceShadowRay(const geometry::GRay& ray, geometry::GHit& hit, const geometry::GBody* ignore = 0);
	private:
		GCamera *m_camera;
		std::list<geometry::GBody*> m_objects;
		cimg_library::CImg<unsigned char> *m_render_buffer;

	};
}