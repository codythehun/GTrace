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
#include "GRaytracer.h"

using namespace cimg_library;
using namespace gtrace;
using namespace gtrace::geometry;
using namespace Eigen;

const unsigned int RESOLUTION_X = 1280;
const unsigned int RESOLUTION_Y = 720;

int _tmain(int argc, _TCHAR* argv[])
{
	GCamera *cam = new GCamera(RESOLUTION_X, RESOLUTION_Y);
	cam->SetOrientation(-20, 0, 10);

	GSphere* sph = new GSphere(Vector3f(0, -0.6f, 2.0f), 0.4f);
	sph->m_material.m_ambient = Vector3f(0.2f, 0.0f, 0.0f);
	sph->m_material.m_diffuse = Vector3f(1.0f, 0.0f, 0.0f);
	sph->m_material.m_specular = Vector3f(0.8f, 0.8f, 0.8f);
	sph->m_material.m_shininess = 4.0f;
	
	long time = GetTickCount();
	GPlane* plane = new GPlane(Vector3f::UnitY(), -1.0f);
	plane->m_material.m_ambient = Vector3f(0.1f, 0.1f, 0.1f);
	plane->m_material.m_diffuse = Vector3f(0.3f, 0.3f, 0.3f);
	plane->m_material.m_specular = Vector3f(0.2f, 0.2f, 0.2f);
	plane->m_material.m_shininess = 1.0f;

	GRaytracer tracer;
	tracer.SetCamera(cam);
	tracer.AddObject(plane);
	tracer.AddObject(sph);
	tracer.Render("render.bmp");
	
	time = GetTickCount() - time;
	std::cout << "Render time: " << time << " ms";

	
	return 0;
}

