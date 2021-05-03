#ifndef __POINT_HPP__
#define __POINT_HPP__

struct Point {
    Point();
    Point(const double a_x, const double a_y);

    ~Point();


    // Methods
    void Move(const double a_deltaX, const double a_deltaY);

    double DistanceFromOrigin() const;
    void Print() const;
    double DistanceTo(const Point a_other) const;
    Point operator+(const Point a_other) const;
    Point operator-(const Point a_other) const;
    Point operator*(const double a_scalar) const;
    Point operator*=(const double a_scalar);

    // Members
    double m_x;
    double m_y;
};

#endif // __POINT_HPP__