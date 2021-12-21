#ifndef NM_POINT_HXX
#define NM_POINT_HXX


#include <type_traits> // std::is_arithmetic
#include "point.hpp"


namespace nm
{

	template <typename T>
	Point<T>::Point()
	: m_x{}
	, m_y{}
	{
		static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type (a floating point type or an integral type)");
	}

	template <typename T>
	Point<T>::Point(T a_x, T a_y)
	: m_x{a_x}
	, m_y{a_y}
	{
		static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type (a floating point type or an integral type)");
	}

	template <typename T>
	Point<T>& Point<T>::operator+=(const Point<T>& a_other)
	{
		m_x += a_other.m_x;
		m_y += a_other.m_y;
		return *this;
	}


	template <typename T>
	Point<T>& Point<T>::operator-=(const Point<T>& a_other)
	{
		m_x -= a_other.m_x;
		m_y -= a_other.m_y;
		return *this;
	}

	template <typename T>
	Point<T>& Point<T>::operator+=(T a_value)
	{
		return *this += Point<T>(a_value, a_value);
	}


	template <typename T>
	Point<T>& Point<T>::operator-=(T a_value)
	{
		return *this -= Point<T>(a_value, a_value);
	}


	template <typename T>
	Point<T> Point<T>::operator+(const Point<T>& a_other) const
	{
		Point<T> p(*this);
		return p += a_other;
	}
	
	
	template <typename T>
	Point<T> Point<T>::operator-(const Point<T>& a_other) const
	{
		Point<T> p(*this);
		return p -= a_other;
	}


	template <typename T>
	Point<T> Point<T>::operator+(T a_value) const
	{
		Point<T> p(*this);
		return p += Point<T>(a_value, a_value);
	}


	template <typename T>
	Point<T> Point<T>::operator-(T a_value) const
	{
		Point<T> p(*this);
		return p -= Point<T>(a_value, a_value);
	}


	template <typename T>
	Point<T> Point<T>::operator-() const
	{
		Point<T> p(*this);
		p.m_x *= -1;
		p.m_y *= -1;
		return p;
	}
}


#endif // NM_POINT_HXX
