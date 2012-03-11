#pragma once

namespace gtrace
{
namespace material
{
	template<class Color, class Expr>
	class GMap
	{
	public:
		GMap(const Expr& expr): m_expr(expr) {}
		__forceinline Color operator() (float u, float v) 
		{
			return m_expr(u,v);
		}
	private:
		const Expr& m_expr;

	};

	template<class Color, class Lhs, class Rhs>
	class GMap_add
	{
	public:
		GMap_add(const Lhs& lhs, const Rhs& rhs): m_lhs(lhs), m_rhs(rhs) {}
		__forceinline Color operator() (float u, float v)
		{
			return lhs(u,v) + rhs(u,v);
		}
	private:
		const Lhs& m_lhs;
		const Rhs& m_rhs;
	};

	template<class Color, class Lhs, class Rhs>
	class GMap_mul
	{
	public:
		GMap_mul(const Lhs& lhs, const Rhs& rhs): m_lhs(lhs), m_rhs(rhs) {}
		__forceinline Color operator() (float u, float v)
		{
			return lhs(u,v).cwiseProduct(rhs(u,v));
		}
	private:
		const Lhs& m_lhs;
		const Rhs& m_rhs;
	};

	template<class Color>
	class GMap_const
	{
	public:
		GMap_const(const Color& value): m_value(value) {}
		__forceinline Color operator() (float u, float v) { return m_value; }
	private:
		const Color& m_value;
	};

	template<class Color, class Scalar>
	class GMap_scalar
	{
	public:
		GMap_scalar(const Scalar& scalar): m_value(scalar) {}
		__forceinline Color operator() (float u, float v) { return Color(m_value) }
	private:
		const Scalar& m_value;
	};

	template<class Color, class Lhs, class Rhs>
	GMap<Color, GMap_add<Color, Lhs, Rhs> > operator+ (const GMap<Color, Lhs>& lhs, const GMap<Color, Rhs>& rhs)
	{
		return GMap<Color, GMap_add<Color, Lhs, Rhs> >(lhs, rhs);
	}


}
}

GMap clouds = Noise(1) * 1/8 + Noise(2) * 1/4 + 