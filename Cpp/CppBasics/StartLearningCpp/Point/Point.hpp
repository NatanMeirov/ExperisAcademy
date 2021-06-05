#ifndef __POINT_HPP__
#define __POINT_HPP__

#include <cstddef> // size_t

// Polar representation
class Point {
public:

    Point();
    Point(const double a_x, const double a_y);
    Point(const double a_x, const double a_y, const char* a_nameOfPoint);
    Point(const Point& a_other);
    Point& operator=(const Point& a_other);
    ~Point();

    // Methods
    void Move(const double a_deltaX, const double a_deltaY);
    Point& operator*=(const double a_scalar);

    // Const Methods
    double GetX() const;
    double GetY() const;
    double DistanceFromOrigin() const;
    void Print() const;
    void Print(const char a_prefixCharacter) const;
    void Print(const char a_prefixCharacter, const size_t a_printPrefixTimes) const;
    void Print(const size_t a_printPointTimes) const;
    double DistanceTo(const Point& a_other) const;
    Point operator+(const Point& a_other) const;
    Point operator-(const Point& a_other) const;
    Point operator*(const double a_scalar) const;

private:

    // Defines
    static const int MAX_STR_LENGTH = 47; // C++ way to do defines WITHOUT using the preprocessor
    static const int TERMINATOR_SIGN_SIZE = sizeof('\0');

    // Members
    size_t m_radius;
    double m_teta;
    char m_nameOfPoint[MAX_STR_LENGTH + TERMINATOR_SIGN_SIZE];
};

#endif // __POINT_HPP__