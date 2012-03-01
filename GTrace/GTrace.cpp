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
#include "GMaterial.h"

using namespace cimg_library;
using namespace gtrace;
using namespace gtrace::geometry;
using namespace gtrace::material;
using namespace Eigen;

const unsigned int RESOLUTION_X = 1280;
const unsigned int RESOLUTION_Y = 720;

int _tmain(int argc, _TCHAR* argv[])
{
	GCamera *cam = new GCamera(RESOLUTION_X, RESOLUTION_Y);
	cam->SetOrientation(-20, 0, 10);
	//cam->SetOrientation(5, 0, 10);
	//cam->SetPosition(Vector3f(0.0f, -0.95f, 0.0f));
	GSphere* sph = new GSphere(Vector3f(0, -0.6f, 2.0f), 0.4f);
	GMaterial m1, m2;

	m1.m_ambient = Vector3f(0.0f, 0.0f, 0.2f);
	m1.m_diffuse = Vector3f(0.0f, 0.0f, 1.0f);
	m1.m_specular = Vector3f(0.5f, 0.5f, 0.5f);
	m1.m_shininess = 0.5f;
	sph->SetMaterial(&m1);
	
	long time = GetTickCount();
	GPlane* plane = new GPlane(Vector3f::UnitY(), -1.0f);
	auto checker = [] (float u, float v) -> const Vector3f& 
		{
			static Vector3f col1(0.0f, 0.0f, 0.0f);
			static Vector3f col2(0.5f, 0.0f, 0.0f);
			int i = (int)abs(u*2);
			int j = (int)abs(v*2) ;
			int sum = (i + j) % 2;
			if ( (u < 0.0f && j >= 0.0f) || (u >= 0.0f && j < 0.0f)) sum = 1 - sum; 
			return sum == 0 ? col1 : col2;

		};

	m2.m_diffuse.SetMapper(0, checker);
	m2.m_ambient.SetMapper(0, checker);
	m2.m_diffuse.SetValue(Vector3f(0.3f, 0.3f, 0.3f));
	//m2.m_specular = Vector3f(0.5f, 0.5f, 0.5f);
	//m2.m_shininess = 5.0f;
	plane->SetMaterial(&m2);
	
	GRaytracer tracer;
	tracer.SetCamera(cam);
	tracer.AddObject(plane);
	tracer.AddObject(sph);
	tracer.Render("render.bmp");
	
	time = GetTickCount() - time;
	std::cout << "Render time: " << time << " ms";
	char c;
	std::cin >> c;
	
	return 0;
}

