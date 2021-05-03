#include "Point.hpp"
#include <math.h> // sqrt
#include <stdio.h> // printf


Point::Point()
: m_x(0), m_y(0) {
}


Point::Point(double a_x, double a_y) {
    this->m_x = a_x;
    this->m_y = a_y;
}


Point::~Point() {
}


double Point::DistanceToOrigin() {
    return sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y)); // sqrt(x^2 + y^2)
}


double Point::DistanceTo(Point a_other) {
    double deltaX = this->m_x - a_other.m_x;
    double deltaY = this->m_y - a_other.m_y;

    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}


void Point::Print() {
    printf("(%.3f, %.3f)\n", this->m_x, this->m_y);
}


Point Point::Add(Point a_other) {
    return Point(this->m_x + a_other.m_x, this->m_y + a_other.m_y);
}


Point Point::Substact(Point a_other) {
    return Point(this->m_x - a_other.m_x, this->m_y - a_other.m_y);
}