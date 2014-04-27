#pragma once
#include<vector>
#include<limits>
#include<Eigen/core>

namespace gtrace {

	template<class T>
	class GSurface
	{
	public:
		struct Pixel {
			T r;
			T g;
			T b;
			T a;

			Pixel() : r(0), g(0), b(0), a(0) {}
			Pixel(const Eigen::Vector3f& vec) {
				r = clamp(vec[0]);
				g = clamp(vec[1]);
				b = clamp(vec[2]);
				a = 0;
			}

		private:
			T clamp(float v) {
				T max(std::numeric_limits<T>::max());
				return max * (v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v));
			}

		};

	private:
		size_t m_width;
		size_t m_height;
		std::vector<Pixel> m_surface;

	public:
		GSurface(size_t width, size_t height);
		void SetDimensions(size_t width, size_t height);
		void SetPixel(size_t x, size_t y, const Pixel& pix);
	};

}
