#include "../Include/Line.hpp"
#include "../Include/Shape.hpp"
#include "../asciicanvas/ascii/point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"


PaintDrawer::Line::Line(const ascii::Point& a_pointA, const ascii::Point& a_pointB, const char a_brush)
: m_pointA(a_pointA)
, m_pointB(a_pointB){
    this->IDrawable::SetBrush(a_brush);
}


void PaintDrawer::Line::Move(const int a_xOffset, const int a_yOffset) {
    // TODO - add the offset of x and y to both a and b points
}


void PaintDrawer::Line::Draw(ascii::Canvas& a_canvas) const {
    // TODO - check if is flipped, and draw the line on the canvas
}


PaintDrawer::ICloneable* PaintDrawer::Line::Clone() const {
    return new PaintDrawer::Line(*this);
}