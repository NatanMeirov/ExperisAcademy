#include <iostream> // std::cout
#include <unistd.h>
#include "./asciicanvas/ascii/canvas.hpp"
#include "./asciicanvas/ascii/point.hpp"


void LaunchMovingSpaceship(int a_xOfCanvas, int a_yOfCanvas);
int CoordMiddle(int a_limitOfCoord);
void DrawSpaceshipOnCanvas(ascii::Canvas& a_canvas, const ascii::Point& a_start, const double& a_spaceWidth);

// Test functions:
void PrintCanvasToCheckIfValidCanvasSize();


int main() {

    // PrintCanvasToCheckIfValidCanvasSize(); // 129, 20

    LaunchMovingSpaceship(129, 20);

    return 0;
}


// void PrintCanvasToCheckIfValidCanvasSize() {
//     ascii::Canvas canvas(129, 20);
//     ascii::DrawLine(canvas, '=', ascii::Point(0, 19), ascii::Point(128, 19));
//     canvas.Print(std::cout);
// }



int CoordMiddle(int a_limitOfCoord) {
    return a_limitOfCoord / 2;
}


void LaunchMovingSpaceship(int a_xOfCanvas, int a_yOfCanvas) {
    int lastValidX = a_xOfCanvas, lastValidY = a_yOfCanvas;
    int spaceWidth = 5;
    int xCoordOffset = 1;
    ascii::Canvas emptyInitializedCanvas(lastValidX, lastValidY); // To AVOID using canvas.Clear() method (reason: seg fault code)

    while(true) {
        system("clear"); // To clear the screen
        ascii::Canvas canvas(emptyInitializedCanvas);
        DrawSpaceshipOnCanvas(canvas, ascii::Point(spaceWidth + xCoordOffset, CoordMiddle(lastValidY)), spaceWidth);
        canvas.Print(std::cout);
        // canvas.Clear();
        ++xCoordOffset;

        if(xCoordOffset > lastValidX) {
            break;
        }

        usleep(8000); // To freeze the picture so it will be presented as a moving animation
    }
}


void DrawSpaceshipOnCanvas(ascii::Canvas& a_canvas, const ascii::Point& a_start, const double& a_spaceWidth) {
    int radiusCounter = 1;
    while((a_spaceWidth / radiusCounter) > 0) {
        ascii::DrawCircle(a_canvas, '*', a_start, a_spaceWidth / radiusCounter);
        radiusCounter *= 2;
    }

    int xCounter = 1;
    int yCounter = 1;

    while((a_spaceWidth / xCounter) > 0) {
        ascii::DrawLine(a_canvas, '#', ascii::Point(a_start.GetX() - (a_spaceWidth / xCounter), a_start.GetY() + (a_spaceWidth + yCounter)), ascii::Point(a_start.GetX() + (a_spaceWidth / xCounter), a_start.GetY() + (a_spaceWidth + yCounter)));
        xCounter *= 2;
        ++yCounter;
    }
}