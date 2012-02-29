#pragma once

#include <Eigen/core>
#include "GGeometry.h"
#include <Eigen\core>

namespace gtrace 
{
namespace material
{
	template<class T>
	class GMapper
	{
	public:
		virtual const T& operator() (float u, float v) { return T();}
	};

	template<class T>
	class GParameter
	{
	public:
		typedef T value_type;

		GParameter(): m_mapped(false), m_map_channel(0) {}
		GParameter(const T& value): m_mapped(false), m_value(value), m_map_channel(0) {}
		GParameter(int map_channel, const GMapper<T>& mapper) : m_mapped(true), m_mapper(mapper), m_map_channel(map_channel) {}

		const T& Resolve()
		{
			if(!m_mapped) return m_value;
			//return m_mapper(hit.u, hit.v);
		}

		const T& operator()()
		{
			return Resolve();
		}

		void SetMapper(int channel, const GMapper<T>& mapper) 
		{
			m_map_channel = channel;
			m_mapper = mapper; 
			m_mapped = true; 
		}

		void SetValue(const T& value)
		{
			m_value = value;
		}


	protected:
		bool m_mapped;

		GMapper<T> m_mapper;
		int m_map_channel;

		T m_value;
	};

	typedef GParameter<float> GParameterf;
	typedef GParameter<Eigen::Vector3f> GParameter3f;

	class GMaterial
	{
	public:
		GParameter3f m_diffuse;
		GParameter3f m_ambient;
		GParameter3f m_specular;
		GParameterf m_transparency;
		GParameterf m_reflection;
		GParameterf m_refraction_index;
		GParameterf m_freshnel_coeff;
		GParameterf m_shininess;
	};
}
}