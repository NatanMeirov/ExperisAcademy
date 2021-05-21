#include "../Include/ShapesGroup.hpp"
#include "../Include/IDrawable.hpp"
#include "../Include/Shape.hpp"
#include "../asciicanvas/ascii/canvas.hpp"


PaintDrawer::ShapesGroup::ShapesGroup(const char a_brush)
: m_shapes() {
    this->m_brush = a_brush; // CANNOT be initialized within the MIL
}


PaintDrawer::ShapesGroup::ShapesGroup(const ShapesGroup& a_other)
: m_shapes() {
    // Every Shape pointer of the other list will be copied using the ICloneable's Clone() function
    // (Must do it that way to avoid using RTTI (to know which Derived Class is each Shape to know which we should create and add to the new list...))

    this->AddCopiesFromOtherGroup(a_other);
    this->m_brush = a_other.m_brush;
}


PaintDrawer::ShapesGroup& PaintDrawer::ShapesGroup::operator=(const ShapesGroup& a_other) {
    this->ClearTheGroup();
    this->AddCopiesFromOtherGroup(a_other);
    this->m_brush = a_other.m_brush;

    return *this;
}


PaintDrawer::ShapesGroup::~ShapesGroup() {
    this->ClearTheGroup();
}


void PaintDrawer::ShapesGroup::AddToGroup(Shape* a_shape) {
    this->m_shapes.push_back(a_shape);
}


void PaintDrawer::ShapesGroup::AddCopiesFromOtherGroup(const ShapesGroup& a_other) {
    std::list<PaintDrawer::Shape*>::const_iterator otherCurrentIterator = a_other.m_shapes.begin();
    std::list<PaintDrawer::Shape*>::const_iterator otherEndIterator = a_other.m_shapes.end();

    while(otherCurrentIterator != otherEndIterator) {
        Shape* shapeCopy = static_cast<Shape*>((*otherCurrentIterator)->Clone());
        this->m_shapes.push_back(shapeCopy);

        ++otherCurrentIterator;
    }
}


PaintDrawer::Shape* PaintDrawer::ShapesGroup::RemoveLastFromGroup() {
    if(this->m_shapes.empty()) {
        return nullptr;
    }
    Shape* shapeToRemove = this->m_shapes.back();
    this->m_shapes.pop_back();

    return shapeToRemove;
}


void PaintDrawer::ShapesGroup::ClearTheGroup() {
    std::list<PaintDrawer::Shape*>::iterator currentIterator = this->m_shapes.begin();
    std::list<PaintDrawer::Shape*>::iterator endIterator = this->m_shapes.end();

    while(currentIterator != endIterator) {
        delete *currentIterator;
        ++currentIterator;
    }

    this->m_shapes.clear();
}


void PaintDrawer::ShapesGroup::MoveTheGroup(const int a_xOffset, const int a_yOffset) {
    std::list<PaintDrawer::Shape*>::const_iterator currentIterator = this->m_shapes.begin();
    std::list<PaintDrawer::Shape*>::const_iterator endIterator = this->m_shapes.end();

    while(currentIterator != endIterator) {
        Shape* currentShape = *currentIterator;
        currentShape->Move(a_xOffset, a_yOffset);

        ++currentIterator;
    }
}


void PaintDrawer::ShapesGroup::Draw(ascii::Canvas& a_canvas) const {
    std::list<PaintDrawer::Shape*>::const_iterator currentIterator = this->m_shapes.begin();
    std::list<PaintDrawer::Shape*>::const_iterator endIterator = this->m_shapes.end();

    while(currentIterator != endIterator) {
        Shape* currentShape = *currentIterator;
        char originalShapeBrush = currentShape->GetBrush(); // Saves its original brush first, to AVOID changing its own brush
        currentShape->SetBrush(this->m_brush);
        currentShape->Draw(a_canvas);
        currentShape->SetBrush(originalShapeBrush);

        ++currentIterator;
    }
}