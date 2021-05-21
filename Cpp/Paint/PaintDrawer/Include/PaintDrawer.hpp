// The Main API to draw shapes and group of shapes on a canvas
#ifndef __PAINTDRAWER_PAINTDRAWER_HPP__
#define __PAINTDRAWER_PAINTDRAWER_HPP__


#include "../asciicanvas/ascii/canvas.hpp"
#include "../Include/IDrawable.hpp"
#include <vector>


namespace PaintDrawer {

class PaintDrawer {
public:
    // TODO - AddLine, AddCircle, AddRectangle, {or instead - add drawable item and let the user create its own shapes and groups by himself} delete last added, ...


private:
    ascii::Canvas m_canvas;
    std::vector<IDrawable*> m_drawablesCollection;
};

} // PaintDrawer


#endif // __PAINTDRAWER_PAINTDRAWER_HPP__