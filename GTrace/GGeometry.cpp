#pragma once

#include "stdafx.h"
#include <Eigen/core>
#include <Eigen/geometry>
#include "GGeometry.h"
#include <random>

using namespace Eigen;

static const float DEG2RAD = 2 * M_PI / 360.0;
static const float RAD2DEG = 360 / (2 * M_PI);

namespace gtrace
{
namespace geometry
{
	float Deg2Rad(float degree)
	{
		return degree * DEG2RAD;
	}
	float Rad2Deg(float radian)
	{
		return radian * RAD2DEG;
	}

	float clamp(float value, float min, float max)
	{ 
		return value < min ? min : (value > max ? max : value); 
	}

	float random(float min, float max)
	{
		return min + rand() / (float)RAND_MAX * (max - min);
	}

	

	GRay::GRay(): m_origin(Vector3f::Zero()), m_direction(Vector3f::Zero()) {}

	GRay::GRay(Vector3f direction): m_origin(Vector3f::Zero()), m_direction(direction) {}

	GRay::GRay(Vector3f origin, Vector3f direction): m_origin(origin), m_direction(direction) {}

	GBody::GBody(Vector3f position):m_position(position) {}

}
}
