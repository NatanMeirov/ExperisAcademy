#ifndef __PAINTDRAWER_CIRCLE_HPP__
#define __PAINTDRAWER_CIRCLE_HPP__


#include "Shape.hpp"
#include "ICloneable.hpp"
#include "Point.hpp"
#include "../asciicanvas/ascii/canvas.hpp"


namespace PaintDrawer {

class Circle : public Shape {
public:
    Circle(const PaintDrawer::Point& a_center, const double a_radius, const char a_brush);
    Circle(const PaintDrawer::Circle& a_other);
    Circle& operator=(const PaintDrawer::Circle& a_other);
    ~Circle() = default;

    virtual void MoveBy(const int a_xOffset, const int a_yOffset) override;
    virtual void Draw(ascii::Canvas& a_canvas) const override;
    virtual ICloneable* Clone() const override;

private:
    ascii::Point m_center;
    double m_radius;
};

}


#endif // __PAINTDRAWER_CIRCLE_HPP__