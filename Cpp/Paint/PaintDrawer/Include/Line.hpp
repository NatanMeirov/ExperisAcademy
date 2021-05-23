#ifndef __PAINTDRAWER_LINE_HPP__
#define __PAINTDRAWER_LINE_HPP__


#include "Shape.hpp"
#include "ICloneable.hpp"
#include "Point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"


namespace PaintDrawer {

class Line : public Shape {
public:
    Line(const PaintDrawer::Point& a_pointA, const PaintDrawer::Point& a_pointB, const char a_brush);
    Line(const Line& a_other);
    Line& operator=(const Line& a_other);
    ~Line() = default;

    virtual void MoveBy(const int a_xOffset, const int a_yOffset) override;
    virtual void Draw(ascii::Canvas& a_canvas) const override;
    virtual ICloneable* Clone() const override;

private:
    ascii::Point m_pointA;
    ascii::Point m_pointB;
};

} // PaintDrawer


#endif // __PAINTDRAWER_LINE_HPP__