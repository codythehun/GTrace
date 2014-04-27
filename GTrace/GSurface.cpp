#include "stdafx.h"
#include "GSurface.h"

namespace gtrace {

	template<class T>
	void GSurface<T>:: SetDimensions(size_t width, size_t height) {
		m_width = width;
		m_height = height;
		m_surface.resize(width * height * 4);
	}

	template<class T>
	GSurface<T>::GSurface(size_t width, size_t height) : m_width(width), m_height(height), m_surface(width * height * 4)
	{
	}

	template<class T>
	void GSurface<T>::SetPixel(size_t x, size_t y, const Pixel& pix)
	{
		m_surface[y * m_width + x] = pix;
	}

	template class GSurface<unsigned char>;
}