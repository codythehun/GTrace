// GTrace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CImg/CImg.h>
#include "GCamera.h"
#include "Eigen/core"

using namespace cimg_library;
using namespace gtrace;
using namespace Eigen;

int _tmain(int argc, _TCHAR* argv[])
{
	CImg<unsigned char> render_buffer(640, 480, 1, 3);
	GCamera cam;
	unsigned int col[3];

	for(unsigned int x = 0; x < 640; ++x)
	{
		for(unsigned int y =0; y < 480; ++y)
		{
			Vector3f ray = cam.GetRayForPosition(x, y);
			float c = (Vector3f::UnitX().dot(ray) + 1.0f) / 2.0f;
			col[0]=col[1]=col[2]=abs(c)*255;
			render_buffer.draw_point(x, y, col);
		}
	}
	render_buffer.save_bmp("render.bmp");

	
	return 0;
}

