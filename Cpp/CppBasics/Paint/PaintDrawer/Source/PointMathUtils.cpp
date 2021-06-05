#include "../Include/PointMathUtils.hpp"
#include "../asciicanvas/ascii/point.hpp"


void PaintDrawer::PointMathUtils::SwapXCoordOfPoints(ascii::Point &a_pointA, ascii::Point &a_pointB) {
    int aX = a_pointA.GetX();
    int bX = a_pointB.GetX();

    a_pointA.Set(bX, a_pointA.GetY());
    a_pointB.Set(aX, a_pointB.GetY());
}


void PaintDrawer::PointMathUtils::SwapYCoordOfPoints(ascii::Point &a_pointA, ascii::Point &a_pointB) {
    int aY = a_pointA.GetY();
    int bY = a_pointB.GetY();

    a_pointA.Set(a_pointA.GetX(), bY);
    a_pointB.Set(a_pointB.GetX(), aY);
}