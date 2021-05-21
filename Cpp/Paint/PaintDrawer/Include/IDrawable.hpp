#ifndef __PAINTDRAWER_IDRAWABLE_HPP__
#define __PAINTDRAWER_IDRAWABLE_HPP__


#include "../asciicanvas/ascii/canvas.hpp"


namespace PaintDrawer {

// A polymorphic base class for all drawable objects
class IDrawable {
public:
    virtual ~IDrawable() = default; // A MUST to destroy every Derived Class objects
    virtual void Draw(ascii::Canvas& a_canvas) const = 0;
    void SetBrush(const char a_brush) { this->m_brush = a_brush; }
    char GetBrush() const { return this->m_brush; }

private:
    char m_brush;
};

} // PaintDrawer


#endif // __PAINTDRAWER_IDRAWABLE_HPP__