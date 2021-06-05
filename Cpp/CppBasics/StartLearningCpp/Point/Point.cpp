#include "Point.hpp"
#include <cstddef> // size_t
#include <cstdio> // putchar
#include <cstring> // memcpy, strcpy
#include <cmath> // sqrt, cos, sin, atan
#include <iostream> // std::cout, std::endl
#include <stdexcept> // std::invalid_argument


static void SafeStrcpyWrapper(char* a_destination, const char* a_source, const size_t a_strLengthLimit, const size_t a_strTerminateSignSize) {
    if(!a_source) {
        throw std::invalid_argument("Null Pointer Error");
    }

    size_t sourceStrLength = strlen(a_source);
    if(sourceStrLength > a_strLengthLimit - a_strTerminateSignSize) {
        throw std::invalid_argument("Wrong Size Error");
    }

    memcpy(a_destination, a_source, sourceStrLength);
}


static size_t CalculateRadius(const double a_x, const double a_y) {
    return sqrt((a_x * a_x) + (a_y * a_y));
}


static double CalculateTeta(const double a_x, const double a_y) {
    return atan(a_y / a_x);
}


Point::Point()
: m_radius(0), m_teta(0) {
    strcpy(this->m_nameOfPoint, "Unnamed point");
}


Point::Point(const double a_x, const double a_y)
: m_radius(CalculateRadius(a_x, a_y))
, m_teta(CalculateTeta(a_x, a_y)) {
    strcpy(this->m_nameOfPoint, "Unnamed point");
}


Point::Point(const double a_x, const double a_y, const char* a_nameOfPoint)
: m_radius(CalculateRadius(a_x, a_y))
, m_teta(CalculateTeta(a_x, a_y)) {
    SafeStrcpyWrapper(this->m_nameOfPoint, a_nameOfPoint, Point::MAX_STR_LENGTH, Point::TERMINATOR_SIGN_SIZE);
}


Point::Point(const Point& a_other)
: m_radius(a_other.m_radius)
, m_teta(a_other.m_teta) {
    // Adding ' to the end of the point's name:
    size_t otherPointLength = strlen(a_other.m_nameOfPoint);
    if(otherPointLength == Point::MAX_STR_LENGTH) /* It cannot be more then the MAX */ {
        throw std::out_of_range("Out of bound limit - cannot add another tag (') to the end of the point's name");
    }

    memcpy(this->m_nameOfPoint, a_other.m_nameOfPoint, otherPointLength); // Other point's name already had been checked in its own initialization part
    this->m_nameOfPoint[otherPointLength] = '\'';
    this->m_nameOfPoint[++otherPointLength] = '\0';
}


Point::~Point() {
}


double Point::GetX() const {
    return this->m_radius * cos(this->m_teta);
}


double Point::GetY() const {
    return this->m_radius * sin(this->m_teta);
}


double Point::DistanceFromOrigin() const {
    return this->m_radius;
    // return sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y)); // sqrt(x^2 + y^2)
}


double Point::DistanceTo(const Point& a_other) const {
    double deltaX = this->GetX() - a_other.GetX();
    double deltaY = this->GetY() - a_other.GetY();

    return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}


/*
void Point::Move(const double a_deltaX, const double a_deltaY) {
    this->m_x += a_deltaX;
    this->m_y += a_deltaY;
}
*/

void Point::Print() const {
    std::cout << "[" << this->m_nameOfPoint << "]"<<  "(" <<  this->GetX() << ", " << this->GetY() << ")";
}


void Point::Print(const char a_prefixCharacter) const {
    putchar(a_prefixCharacter);
    this->Print();
}


void Point::Print(const char a_prefixCharacter, const size_t a_printPrefixTimes) const {
    for(size_t i = 0; i < a_printPrefixTimes; ++i) {
        putchar(a_prefixCharacter);
    }
    this->Print();
}


void Point::Print(const size_t a_printPointTimes) const {
    for(size_t i = 0; i < a_printPointTimes; ++i) {
        this->Print();
        std::cout << ", ";
    }
}


Point Point::operator+(const Point& a_other) const {
    return Point(this->GetX() + a_other.GetX(), this->GetY() + a_other.GetY());
}


Point Point::operator-(const Point& a_other) const {
    return Point(this->GetX() - a_other.GetX(), this->GetY() - a_other.GetY());
}


Point Point::operator*(const double a_scalar) const {
    return Point(this->GetX() * a_scalar, this->GetY() * a_scalar);
}

/*
Point& Point::operator*=(const double a_scalar) {
    this->m_x *= a_scalar;
    this->m_y *= a_scalar;
    return *this;
}
*/