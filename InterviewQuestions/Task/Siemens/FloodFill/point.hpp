#ifndef NM_POINT_HPP
#define NM_POINT_HPP


namespace nm
{

    // Concept of T: T must be an arithmetic type (a floating point type or an integral type)
    template <typename T = int>
    class Point
    {
    public:
        Point();
        Point(T a_x, T a_y); // Most of the valid types are less or equal to 8 bytes, so copy is the best choise
        Point(const Point<T>& a_other) = default;
        Point<T>& operator=(const Point<T>& a_other) = default;
        ~Point() = default;

        Point<T>& operator+=(const Point<T>& a_other);
        Point<T>& operator-=(const Point<T>& a_other);
        Point<T>& operator+=(T a_value);
        Point<T>& operator-=(T a_value);
        Point<T> operator+(const Point<T>& a_other) const;
        Point<T> operator-(const Point<T>& a_other) const;
        Point<T> operator+(T a_value) const;
        Point<T> operator-(T a_value) const;
        Point<T> operator-() const;

        T X() const { return m_x; }
        T Y() const { return m_y; }

    private:
        T m_x;
        T m_y;
    };

} // nm


#include "point.hxx"


#endif // NM_POINT_HPP
