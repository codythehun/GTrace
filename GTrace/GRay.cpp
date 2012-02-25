
#include "Stdafx.h"
#include "GRay.h"
#include <Eigen/core>

using namespace Eigen;

namespace gtrace
{

	GRay::GRay(): m_origin(Vector3f::Zero()), m_direction(Vector3f::Zero()) {}

	GRay::GRay(Vector3f direction): m_origin(Vector3f::Zero()), m_direction(direction) {}

	GRay::GRay(Vector3f origin, Vector3f direction): m_origin(origin), m_direction(direction) {}

}