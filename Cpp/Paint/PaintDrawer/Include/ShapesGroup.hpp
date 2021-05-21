#ifndef __PAINTDRAWER_SHAPESGROUP_HPP__
#define __PAINTDRAWER_SHAPESGROUP_HPP__


#include "../Include/IDrawable.hpp"
#include "../Include/Shape.hpp"
#include "../asciicanvas/ascii/canvas.hpp"
#include <list>


namespace PaintDrawer {

// Note: Every added Shape MUST implement the ICloneable's Clone() method to be able to copy itself! (Used in the Copy C'tor, Copy-Assignment and AddCopiesFromOtherGroup() method)
class ShapesGroup : public IDrawable {
public:
    ShapesGroup(const char a_groupBrush);
    ShapesGroup(const ShapesGroup& a_other);
    ShapesGroup& operator=(const ShapesGroup& a_other);
    ~ShapesGroup();

    // AddToGroup - adds a new Shape pointer to the shapes collection
    void AddToGroup(Shape* a_shape);
    void AddCopiesFromOtherGroup(const ShapesGroup& a_other);
    Shape* RemoveLastFromGroup(); // Returns nullptr if the group is empty
    void ClearTheGroup(); // Deletes all the Shapes in the group and clears the group
    void MoveTheGroup(const int a_xOffset, const int a_yOffset);
    //TODO: ADD a wrapping rectangle to represent the group's space in the canvas (need to check every shape's coors after add and remove if the rectangle has changed!) and EVERY MOVE SHOULD handle it (?)
    //TODO: FlipGroupHorizontally()
    //TODO: FlipGroupVertically()
    virtual void Draw(ascii::Canvas& a_canvas) const override;

private:
    std::list<PaintDrawer::Shape*> m_shapes;
};

} // PaintDrawer


#endif // __PAINTDRAWER_SHAPESGROUP_HPP__