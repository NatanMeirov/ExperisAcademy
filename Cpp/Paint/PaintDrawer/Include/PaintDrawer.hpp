// The Main API to draw shapes and group of shapes on a canvas
#ifndef __PAINTDRAWER_PAINTDRAWER_HPP__
#define __PAINTDRAWER_PAINTDRAWER_HPP__


#include <cstddef> // size_t
#include <ostream> // std::ostream
#include "../asciicanvas/ascii/canvas.hpp"
#include "../Include/IDrawable.hpp"
#include <list>


namespace PaintDrawer {

// Warning: all added pointers MUST BE HEAP ALLOCATED! While Clearing the Draw or destroying an instance of PaintDrawer - every pointer is deallocated (and an error would occur if a stack allocated object's address is included in the PaintDrawer)
class PaintDrawer {
public:
    PaintDrawer(const size_t a_drawWidth, const size_t a_drawHeight);
    PaintDrawer(const PaintDrawer& a_other) = default; // BUG: (known bug) Shallow copy is occurred! Avoid coping a PaintDrawer instance!
    PaintDrawer& operator=(const PaintDrawer& a_other) = default; // BUG: (known bug) Same as above
    ~PaintDrawer();

    void AddToDraw(IDrawable* a_drawable); // Throws std::invalid_argument if pointer is nullptr (NULL)
    IDrawable* RemoveByIndex(const size_t a_index); // Counted from 1 (first added) to n (last added), Returns nullptr if the group is empty, or if index is out of bounds (and not throwing an exception)
    void DrawAPaint(std::ostream& a_os) const;
    void ClearAllPaint(); // Clears and deletes all the previously added IDrawables instances
    void Clear() const; // Clears the last draw without deleting its previously added IDrawables instances
    IDrawable* GetDrawableToModifyByIndex(const size_t a_index); // Note: a fatal change of the returned pointer, it's user responsibility | Counted from 1 (first added) to n (last added), Returns nullptr if the group is empty, or if index is out of bounds (and not throwing an exception)

private:
    ascii::Canvas m_canvas;
    std::list<IDrawable*> m_drawablesCollection;
};

} // PaintDrawer


#endif // __PAINTDRAWER_PAINTDRAWER_HPP__