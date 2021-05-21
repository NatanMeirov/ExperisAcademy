#ifndef __PAINTDRAWER_LINE_HPP__
#define __PAINTDRAWER_LINE_HPP__


#include "Shape.hpp"
#include "../asciicanvas/ascii/point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"


namespace PaintDrawer {

class Line : Shape {
public:
    // Line(const int a_xA, const int a_yA, const int a_xB, const int a_yB, const char a_brush); - not supported yet (no need for now)
    Line(const ascii::Point& a_pointA, const ascii::Point& a_pointB, const char a_brush);
    Line(const Line& a_other) = default;
    Line& operator=(const Line& a_other) = default;
    ~Line() = default;

    virtual void Move(const int a_xOffset, const int a_yOffset) override;
    virtual void Draw(ascii::Canvas& a_canvas) const override;
    virtual ICloneable* Clone() const override;

private:
    ascii::Point m_pointA;
    ascii::Point m_pointB;
};

} // PaintDrawer


#endif // __PAINTDRAWER_LINE_HPP__