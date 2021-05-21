// An adapter class (to be used in the PaintDrawer namespace) of the ascii::point class
#ifndef __PAINTDRAWER_POINT_HPP__
#define __PAINTDRAWER_POINT_HPP__


#include "../asciicanvas/ascii/point.hpp"


namespace PaintDrawer {

class Point {
    Point(int a_x, int a_y);
    Point(const Point &a_other) = default;
    Point& operator=(const Point &a_other) = default;
    ~Point() = default;

    void Set(int a_x, int a_y);
    int GetX() const;
    int GetY() const;

private:
    ascii::Point m_wrappedPoint;
};

} // PaintDrawer


#endif // __PAINTDRAWER_POINT_HPP__