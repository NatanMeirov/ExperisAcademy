#include <iostream> // std::cout
#include <unistd.h>
#include "Include/Circle.hpp"
#include "Include/Line.hpp"
#include "Include/ShapesGroup.hpp"
#include "Include/PaintDrawer.hpp"
#include "Include/Point.hpp"


#define TIMES_TO_PRINT 9


// Note: Rectangle is still NOT supported
int main() {
    PaintDrawer::PaintDrawer paintDrawer(60, 15);

    PaintDrawer::Line* lineWall1 = new PaintDrawer::Line(PaintDrawer::Point(0,14), PaintDrawer::Point(0,0), '#');
    PaintDrawer::Line* lineWall2 = new PaintDrawer::Line (PaintDrawer::Point(50,0), PaintDrawer::Point(50,14), '#');

    PaintDrawer::ShapesGroup* group = new PaintDrawer::ShapesGroup('*');
    PaintDrawer::Line* groupLine1 = new PaintDrawer::Line(PaintDrawer::Point(10,7), PaintDrawer::Point(20,7), '~');
    PaintDrawer::Circle* circle = new PaintDrawer::Circle(PaintDrawer::Point(25,7), 4, '+');
    PaintDrawer::Line* groupLine2 = new PaintDrawer::Line(PaintDrawer::Point(30,7), PaintDrawer::Point(40,7), '$');

    group->AddToGroup(groupLine1);
    group->AddToGroup(circle);
    group->AddToGroup(groupLine2);

    paintDrawer.AddToDraw(lineWall1);
    paintDrawer.AddToDraw(group);
    paintDrawer.AddToDraw(lineWall2);


    int counter = 0;
    while(counter < TIMES_TO_PRINT) {
        system("clear");
        paintDrawer.Clear();
        lineWall1->MoveBy(1, 0);
        group->MoveTheGroupBy(1, 0);
        lineWall2->MoveBy(1, 0);

        paintDrawer.DrawAPaint(std::cout);

        sleep(1);
        counter++;
    }

    return 0;
}