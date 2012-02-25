// GTrace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CImg/CImg.h>
#include "GCamera.h"
#include "GPLane.h"
#include "Eigen/core"
#include "windows.h"
#include <iostream>

using namespace cimg_library;
using namespace gtrace;
using namespace Eigen;

int _tmain(int argc, _TCHAR* argv[])
{
	CImg<unsigned char> render_buffer(640, 480, 1, 3);
	GCamera cam;
	unsigned int col[3];
	cam.SetOrientation(-20, 0, 10);
	long time = GetTickCount64();
	GPlane plane(Vector3f::UnitY(), -1.0f);

	for(unsigned int x = 0; x < 640; ++x)
	{
		for(unsigned int y =0; y < 480; ++y)
		{
			GRay ray = cam.GetRayForPosition(x, y);
			float c = 0.0f; //(Vector3f::UnitZ().dot(ray) + 1.0f) / 2.0f;
			
			Vector3f normal;
			float distance;
			if(plane.Intersect(ray, distance, normal))
			{
				
				c = 1.0f - distance / 5.0f;
				if(c < 0.0f ) c= 0.0f;
			}
			col[0]=col[1]=col[2]=abs(c)*255;
			render_buffer.draw_point(x, y, col);
		}
	}
	time = GetTickCount64() - time;
	std::cout << "Render time: " << time << " ms";
	render_buffer.save_bmp("render.bmp");

	
	return 0;
}

