#include "../Include/Shape.hpp"


PaintDrawer::Shape::Shape()
: m_isFlippedHorizontally(false)
, m_isFlippedVertically(false){
}


bool PaintDrawer::Shape::IsFlippedHorizontally() const {
    return this->m_isFlippedHorizontally;
}


bool PaintDrawer::Shape::IsFlippedVertically() const {
    return this->m_isFlippedVertically;
}


void PaintDrawer::Shape::FlipHorizontally() {
    this->m_isFlippedHorizontally = this->m_isFlippedHorizontally ? false : true;
}


void PaintDrawer::Shape::FlipVertically() {
    this->m_isFlippedVertically = this->m_isFlippedVertically ? false : true;
}