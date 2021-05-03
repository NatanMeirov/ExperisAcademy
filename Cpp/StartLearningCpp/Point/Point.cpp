#include "Point.hpp"
#include <math.h> // sqrt
#include <iostream> // std::cout, std::endl


Point::Point()
: m_x(0), m_y(0) {
}


Point::Point(const double a_x, const double a_y) {
    this->m_x = a_x;
    this->m_y = a_y;
}


Point::~Point() {
}


double Point::DistanceFromOrigin() const {
    return sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y)); // sqrt(x^2 + y^2)
}


double Point::DistanceTo(const Point a_other) const {
    double deltaX = this->m_x - a_other.m_x;
    double deltaY = this->m_y - a_other.m_y;

    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}


void Point::Move(const double a_deltaX, const double a_deltaY) {
    this->m_x += a_deltaX;
    this->m_y += a_deltaY;
}


void Point::Print() const {
    std::cout << "(" <<  this->m_x << ", " << this->m_y << ")" << std::endl;
}


Point Point::operator+(const Point a_other) const {
    return Point(this->m_x + a_other.m_x, this->m_y + a_other.m_y);
}


Point Point::operator-(const Point a_other) const {
    return Point(this->m_x - a_other.m_x, this->m_y - a_other.m_y);
}


Point Point::operator*(const double a_scalar) const {
    return Point(this->m_x * a_scalar, this->m_y * a_scalar);
}


Point Point::operator*=(const double a_scalar) {
    return Point(this->m_x * a_scalar, this->m_y * a_scalar);
}