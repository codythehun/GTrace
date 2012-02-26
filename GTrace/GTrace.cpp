// GTrace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <CImg/CImg.h>
#include "GCamera.h"
#include "GPLane.h"
#include "GSphere.h"
#include "Eigen/core"
#include "windows.h"
#include <iostream>

using namespace cimg_library;
using namespace gtrace;
using namespace gtrace::geometry;
using namespace Eigen;

const unsigned int RESOLUTION_X = 1280;
const unsigned int RESOLUTION_Y = 720;
int _tmain(int argc, _TCHAR* argv[])
{
	CImg<unsigned char> render_buffer(RESOLUTION_X, RESOLUTION_Y, 1, 3);
	GCamera cam(RESOLUTION_X, RESOLUTION_Y);
	unsigned int col[3];
	cam.SetOrientation(-20, 0, 10);
	GSphere sph(Vector3f(0, -0.9f, 2.0f), 0.4f);
	Vector3f light_vec = -Vector3f::UnitY();
	long time = GetTickCount64();
	GPlane plane(Vector3f::UnitY(), -1.0f);

	for(unsigned int x = 0; x < RESOLUTION_X; ++x)
	{
		for(unsigned int y =0; y < RESOLUTION_Y; ++y)
		{
			GRay ray = cam.GetRayForPosition(x, y);
			float c = 0.0f; //(Vector3f::UnitZ().dot(ray) + 1.0f) / 2.0f;
			
			Vector3f normal1, normal2;
			float distance1, distance2=0.0f;
			bool plane_hit, sp_hit;
			plane_hit = plane.Intersect(ray, distance1, normal1);
			sp_hit = sph.Intersect(ray, distance2, normal2);
			if (sp_hit)
			{
				c = ( - normal2.dot(light_vec) + 1.0f) /2.0f;
			}
			else if(plane_hit)
			{
				
				c = 1.0f - distance1 / 5.0f;
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

