#include "flood_fill.hpp"

//
// Example 5 - Graphing
//
// A simple, single-variable graphing calculator.
//
// Exercises:
// 1. Instead of a single pixel, use line segments between adjacent points (so "tan(x)" ends up a solid curve)
// 2. Pull the input & output transforms into separate functions
// 3. Add a second math function "g(x)" and plot it on the same set of axes (using your new transform functions)
// 4. Parameterize your transforms so you can adjust the window/zoom of your graph
//

// We're going to need some interesting math functions for this one!
#include <cmath>
#include <vector>

struct Point
{
    int x, y;

    Point& operator+=(const Point& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

double f(double x)
{
    return sin(x);
}

void drawSinus()
{
    // Draw our axes
    DrawLine(0, Height / 2, Width, Height / 2, 1, DarkGray);
    DrawLine(Width / 2, 0, Width / 2, Height, 1, DarkGray);

    for (int p = 0; p < Width; ++p)
    {
        // We'll need to transform from our pixel coordinate system to the usual Cartesian coordinates
        double x = p;

        // First, shift x=0 to the center of the screen (instead of the left edge)
        x -= Width / 2;

        // Scale x down, effectively setting our graph's "Window" to something like w=(-8, 8), h=(-6, 6)
        x /= 50.0;

        // Run the function!
        double y = f(x);

        // Now we have to scale our result back up
        y *= 50;

        // In computer graphics, y increases downward
        // In Cartesian coordinates, y increases upward
        y = -y;

        // Finally, shift it to the center of the screen (instead of the top edge)
        y += Height / 2;

        SetPixel(p, (int)y, LightRed);
    }
}

void drawPolygon(const std::vector<Point>& poly, const Color& color)
{
    Point prev = poly[poly.size() - 1];
    for (const Point& pt : poly)
    {
        DrawLine(prev.x, prev.y, pt.x, pt.y, 1, color);
        prev = pt;
    }
}

void translatePolygon(std::vector<Point>& poly, const Point& basePoint)
{
    Point prev{basePoint};
    for (Point& pt : poly)
    {
        pt += prev;
        prev = pt;
    }
}

void drawScene()
{
    drawPolygon({{30, 30}, {100,70}, {60, 60}, {60, 200}}, LightGreen);
    drawPolygon({
        {209,195},
        {268,385},
        {294,288},
        {373,423},
        {470,205},
        {389,292},
        {403,313},
        {256,271},
        {394,167},
        {662,246},
        {347,146},
        {518,54},
        {581,8},
        {54,249}
        }, DarkGray);

    int step = 20;
    std::vector<Point> poly{
        { 3*step, 0},
        { 0, 7*step},
        { 7*step, 0},
        { 0, -4*step},
        { -step, 0},
        { 0, 3*step},
        { -5*step, 0},
        { 0, -4*step},
        { 3*step, 0},
        { 0, 2*step},
        { -1*step, 0},
        { 0, -1*step},
        { -1*step, 0},
        { 0, 2*step},
        { 3*step, 0},
        { 0, -3*step},
        { 3*step, 0},
        { 0, 6*step},
        { -9*step, 0},
        { 0, -7*step},
        { -1*step, 0},
        { 0, 1*step},
        { -1*step, 0},
        { 0, -2*step},
    };
    translatePolygon(poly, {500,300});
    drawPolygon(poly, Blue);

     // A rectangle added by Natan Meirov for testing:
    drawPolygon({
        {(Width / 4) - 20, (Height / 4) - 20},
        {(Width / 4) + 20, (Height / 4) - 20 },
        {(Width / 4) + 20, (Height / 4) + 20 },
        {(Width / 4) - 20, (Height / 4) + 20 }
        }, LightMagenta);
}

int main()
{
    drawScene();
    floodFill((Width / 2) - 5, (Height / 2) + 10, MakeColor(255, 0, 175));
    floodFill((Width / 4) - 11, (Height / 4) + 19, Blue);
    floodFill(212, 200, Green);
    floodFill(35, 35, Cyan);
    floodFill(510, 310, White);

    return 0;
}
