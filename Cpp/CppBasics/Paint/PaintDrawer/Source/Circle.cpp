#include "../Include/Circle.hpp"
#include "../Include/Point.hpp"
#include "../asciicanvas/ascii/point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"
#include "../Include/PointMathUtils.hpp"


PaintDrawer::Circle::Circle(const PaintDrawer::Point& a_center, const double a_radius, const char a_brush)
: m_center(ascii::Point(a_center.GetX(), a_center.GetY()))
, m_radius(a_radius){
    this->SetBrush(a_brush);
}


PaintDrawer::Circle::Circle(const PaintDrawer::Circle& a_other)
: m_center(a_other.m_center)
, m_radius(a_other.m_radius){
    this->SetBrush(a_other.GetBrush());
}

PaintDrawer::Circle& PaintDrawer::Circle::operator=(const Circle& a_other) {
    this->m_center = a_other.m_center;
    this->m_radius= a_other.m_radius;
    this->SetBrush(a_other.GetBrush());

    return *this;
}


void PaintDrawer::Circle::MoveBy(const int a_xOffset, const int a_yOffset) {
    this->m_center.Set(this->m_center.GetX() + a_xOffset, this->m_center.GetY() + a_yOffset);
}


void PaintDrawer::Circle::Draw(ascii::Canvas& a_canvas) const {
    ascii::DrawCircle(a_canvas, this->GetBrush(), this->m_center, this->m_radius);
}


PaintDrawer::ICloneable* PaintDrawer::Circle::Clone() const {
    return new PaintDrawer::Circle(*this);
}