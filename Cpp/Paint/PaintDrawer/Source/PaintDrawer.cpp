#include "../Include/PaintDrawer.hpp"
#include <cstddef> // size_t
#include <stdexcept> // std::invalid_argument
#include <ostream> // std::ostream
#include "../asciicanvas/ascii/canvas.hpp"
#include "../Include/IDrawable.hpp"
#include <list>


PaintDrawer::PaintDrawer::PaintDrawer(const size_t a_drawWidth, const size_t a_drawHeight)
: m_canvas(a_drawWidth, a_drawHeight)
, m_drawablesCollection(){
}


PaintDrawer::PaintDrawer::~PaintDrawer() {
    this->ClearAllPaint();
}


void PaintDrawer::PaintDrawer::AddToDraw(IDrawable* a_drawable) {
    if(!a_drawable) {
        throw std::invalid_argument("Null Pointer Error");
    }

    this->m_drawablesCollection.push_back(a_drawable);
}


PaintDrawer::IDrawable* PaintDrawer::PaintDrawer::RemoveByIndex(const size_t a_index){
    if(this->m_drawablesCollection.empty()) {
        return nullptr;
    }

    if(a_index > this->m_drawablesCollection.size() || a_index == 0) {
        return nullptr;
    }

    std::list<IDrawable*>::iterator currentIterator = this->m_drawablesCollection.begin();
    size_t counter = 1; // Starting from 1

    while(counter < a_index) {
        ++currentIterator;
        ++counter;
    }

    IDrawable* drawableToRemove = *currentIterator;
    this->m_drawablesCollection.erase(currentIterator);

    return drawableToRemove;
}


void PaintDrawer::PaintDrawer::DrawAPaint(std::ostream& a_os) const{
    std::list<IDrawable*>::const_iterator currentIterator = this->m_drawablesCollection.begin();
    std::list<IDrawable*>::const_iterator endIterator = this->m_drawablesCollection.end();

    while(currentIterator != endIterator) {
        IDrawable* currentDrawable = *currentIterator;
        currentDrawable->Draw(const_cast<ascii::Canvas&>(this->m_canvas));

        ++currentIterator;
    }

    this->m_canvas.Print(a_os);
}


void PaintDrawer::PaintDrawer::ClearAllPaint(){
    if(this->m_drawablesCollection.empty()) {
        return;
    }

    std::list<IDrawable*>::iterator currentIterator = this->m_drawablesCollection.begin();
    std::list<IDrawable*>::iterator endIterator = this->m_drawablesCollection.end();

    while(currentIterator != endIterator) {
        delete *currentIterator;
        ++currentIterator;
    }

    this->m_drawablesCollection.clear();
    this->m_canvas.Clear();
}


void PaintDrawer::PaintDrawer::Clear() const {
    const_cast<ascii::Canvas&>(this->m_canvas).Clear();
}


PaintDrawer::IDrawable* PaintDrawer::PaintDrawer::GetDrawableToModifyByIndex(const size_t a_index) {
    if(this->m_drawablesCollection.empty()) {
        return nullptr;
    }

    if(a_index > this->m_drawablesCollection.size() || a_index == 0) {
        return nullptr;
    }

    std::list<IDrawable*>::iterator currentIterator = this->m_drawablesCollection.begin();
    size_t counter = 1; // Starting from 1

    while(counter < a_index) {
        ++currentIterator;
        ++counter;
    }

    return *currentIterator;
}