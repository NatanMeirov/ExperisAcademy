#ifndef __PAINTDRAWER_POINTMATHUTILS_HPP__
#define __PAINTDRAWER_POINTMATHUTILS_HPP__


#include "../asciicanvas/ascii/point.hpp"


namespace PaintDrawer {

class PointMathUtils {
public:
    static void SwapXCoordOfPoints(ascii::Point& a_pointA, ascii::Point& a_pointB);
    static void SwapYCoordOfPoints(ascii::Point& a_pointA, ascii::Point& a_pointB);
};

} // PaintDrawer


#endif // __PAINTDRAWER_POINTMATHUTILS_HPP__