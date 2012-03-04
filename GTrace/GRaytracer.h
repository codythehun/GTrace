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
		class Options
		{
		public:
			typedef enum Type { SPECULAR=0, SHADOW, REFLECTION, REFRACTION };

			Options(int depth=1) {for(int i=0; i<4; m_depths[i++]=depth); }
			void SetDepth(Type type, int depth) { m_depths[type] = depth; }
			bool Applicable(int depth, Type type) const { return m_depths[type] >= depth; }
			Options& operator ++() { for(int i=0; i<4; ++m_depths[i++]); return *this; }
			Options& operator --() { for(int i=0; i<4; --m_depths[i++]); return *this; }
			
		private:
			int m_depths[4];
		};
		
		GRaytracer();
		~GRaytracer();

		void SetCamera(GCamera* camera);
		void AddObject(geometry::GBody* object);
		void AddLight(geometry::GBody* light);
		void SetOptions(const Options &options) { m_options = options; }
		void Render(std::string file_name);
	protected:
		Eigen::Vector3f TraceRay(const geometry::GRay& ray, int depth = 1);
		bool TraceRay(const geometry::GRay& ray, geometry::GHit& hit);
		bool TraceShadowRay(const geometry::GRay& ray, geometry::GHit& hit, const geometry::GBody* ignore = 0);
	private:
		GCamera *m_camera;
		std::list<geometry::GBody*> m_objects;
		std::list<geometry::GBody*> m_lights;
		cimg_library::CImg<unsigned char> *m_render_buffer;
		Options m_options;

	};
}