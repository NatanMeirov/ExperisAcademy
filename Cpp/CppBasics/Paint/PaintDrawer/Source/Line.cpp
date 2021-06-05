#include "../Include/Line.hpp"
#include "../Include/Point.hpp"
#include "../asciicanvas/ascii/point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"
#include "../Include/PointMathUtils.hpp"


PaintDrawer::Line::Line(const PaintDrawer::Point& a_pointA, const PaintDrawer::Point& a_pointB, const char a_brush)
: m_pointA(ascii::Point(a_pointA.GetX(), a_pointA.GetY()))
, m_pointB(ascii::Point(a_pointB.GetX(), a_pointB.GetY())){
    this->SetBrush(a_brush);
}


PaintDrawer::Line::Line(const PaintDrawer::Line& a_other)
: m_pointA(a_other.m_pointA)
, m_pointB(a_other.m_pointB){
    this->SetBrush(a_other.GetBrush());
}

PaintDrawer::Line& PaintDrawer::Line::operator=(const Line& a_other) {
    this->m_pointA = a_other.m_pointA;
    this->m_pointA = a_other.m_pointB;
    this->SetBrush(a_other.GetBrush());

    return *this;
}


void PaintDrawer::Line::MoveBy(const int a_xOffset, const int a_yOffset) {
    this->m_pointA.Set(this->m_pointA.GetX() + a_xOffset, this->m_pointA.GetY() + a_yOffset);
    this->m_pointB.Set(this->m_pointB.GetX() + a_xOffset, this->m_pointB.GetY() + a_yOffset);
}


void PaintDrawer::Line::Draw(ascii::Canvas& a_canvas) const {
    ascii::Point firstPointOfLineToDraw(this->m_pointA);
    ascii::Point secondPointOfLineToDraw(this->m_pointB);

    if(this->IsFlippedHorizontally()) {
        PointMathUtils::SwapXCoordOfPoints(firstPointOfLineToDraw, secondPointOfLineToDraw);
    }

    if(this->IsFlippedVertically()) {
        PointMathUtils::SwapYCoordOfPoints(firstPointOfLineToDraw, secondPointOfLineToDraw);
    }

    ascii::DrawLine(a_canvas, this->GetBrush(), firstPointOfLineToDraw, secondPointOfLineToDraw);
}


PaintDrawer::ICloneable* PaintDrawer::Line::Clone() const {
    return new PaintDrawer::Line(*this);
}