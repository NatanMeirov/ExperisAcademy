#ifndef __PAINTDRAWER_SHAPE_HPP__
#define __PAINTDRAWER_SHAPE_HPP__


#include "IDrawable.hpp"
#include "ICloneable.hpp"


namespace PaintDrawer {

// A polymorphic base class for all shapes
class Shape : public IDrawable, public ICloneable {
public:
    virtual ~Shape() = default; // A MUST to destroy every Derived Class objects
    virtual void Move(const int a_xOffset, const int a_yOffset) = 0;
    bool IsFlippedHorizontally() const;
    bool IsFlippedVertically() const;
    void FlipHorizontally();
    void FlipVertically();

protected:
    Shape();
    bool m_isFlippedHorizontally;
    bool m_isFlippedVertically;
};

} // PaintDrawer


#endif // __SHAPE_HPP__