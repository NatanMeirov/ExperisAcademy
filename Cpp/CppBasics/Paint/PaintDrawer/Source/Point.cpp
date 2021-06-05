#include "../Include/Point.hpp"
#include "../asciicanvas/ascii/point.hpp"


PaintDrawer::Point::Point(int a_x, int a_y)
: m_wrappedPoint(a_x, a_y) {
}


void PaintDrawer::Point::Set(int a_x, int a_y) {
    this->m_wrappedPoint.Set(a_x, a_y);
}


int PaintDrawer::Point::GetX() const {
    return this->m_wrappedPoint.GetX();
}


int PaintDrawer::Point::GetY() const {
    return this->m_wrappedPoint.GetY();
}