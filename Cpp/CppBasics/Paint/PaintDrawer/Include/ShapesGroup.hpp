#ifndef __PAINTDRAWER_SHAPESGROUP_HPP__
#define __PAINTDRAWER_SHAPESGROUP_HPP__


#include <cstddef> // size_t
#include <stdexcept> // std::invalid_argument
#include "../Include/IDrawable.hpp"
#include "../Include/Shape.hpp"
#include "../asciicanvas/ascii/canvas.hpp"
#include <list>


namespace PaintDrawer {

// Note: Every added Shape MUST implement the ICloneable's Clone() method to be able to copy itself! (Used in the Copy C'tor, Copy-Assignment and AddCopiesFromOtherGroup() method)
// Warning: all added pointers MUST BE HEAP ALLOCATED! While Clearing the Group or destroying an instance of a ShapesGroup - every pointer is deallocated (and an error would occur if a stack allocated object's address is included in the ShapesGroup)
class ShapesGroup : public IDrawable {
public:
    ShapesGroup(const char a_groupBrush);
    ShapesGroup(const ShapesGroup& a_other);
    ShapesGroup& operator=(const ShapesGroup& a_other);
    ~ShapesGroup();

    // AddToGroup - adds a new Shape pointer to the shapes collection
    void AddToGroup(Shape* a_shape); // Throws std::invalid_argument if pointer is nullptr (NULL)
    void AddCopiesFromOtherGroup(const ShapesGroup& a_other);
    Shape* RemoveLastFromGroup(); // Returns nullptr if the group is empty
    void ClearTheGroup(); // Deletes all the Shapes in the group and clears the group
    void MoveTheGroupBy(const int a_xOffset, const int a_yOffset);
    Shape* RemoveFromGroupByIndex(size_t a_index); // Counted from 1 (first added) to n (last added), Returns nullptr if the group is empty, or if index is out of bounds (and not throwing an exception)
    void FlipGroupHorizontally();
    void FlipGroupVertically();
    virtual void Draw(ascii::Canvas& a_canvas) const override;

private:
    std::list<PaintDrawer::Shape*> m_shapes;
};

} // PaintDrawer


#endif // __PAINTDRAWER_SHAPESGROUP_HPP__