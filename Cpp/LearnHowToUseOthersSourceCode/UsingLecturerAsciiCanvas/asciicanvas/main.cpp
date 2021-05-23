#include "ascii/canvas.hpp"
#include <iostream>
#include <unistd.h>


int main() {
    ascii::Canvas canvas(60, 15);

    ascii::DrawCircle(canvas, '*', ascii::Point(7, 7), 7);
    ascii::DrawLine(canvas, '#', ascii::Point(15, 7), ascii::Point(40, 7));
    ascii::DrawLine(canvas, '#', ascii::Point(40, 7), ascii::Point(55, 0));
    ascii::DrawLine(canvas, '#', ascii::Point(40, 7), ascii::Point(55, 14));

    ascii::Canvas canvas2(canvas);

    ascii::DrawCircle(canvas, '#', ascii::Point(4, 4), 1);
    ascii::DrawCircle(canvas, '#', ascii::Point(5, 4), 1);
    ascii::DrawCircle(canvas, 'o', ascii::Point(6, 4), 1);
    ascii::DrawCircle(canvas, '#', ascii::Point(4, 9), 1);
    ascii::DrawCircle(canvas, '#', ascii::Point(5, 9), 1);
    ascii::DrawCircle(canvas, 'o', ascii::Point(6, 9), 1);
    ascii::DrawLine(canvas, '@', 20, 6, 27, 0);
    ascii::DrawLine(canvas, '@', 20, 8, 27, 14);

    ascii::DrawCircle(canvas2, '#', 3, 4, 1);
    ascii::DrawCircle(canvas2, 'O', 4, 4, 1);
    ascii::DrawCircle(canvas2, 'O', 5, 4, 1);
    ascii::DrawCircle(canvas2, '#', 3, 9, 1);
    ascii::DrawCircle(canvas2, 'O', 4, 9, 1);
    ascii::DrawCircle(canvas2, 'O', 5, 9, 1);
    ascii::DrawLine(canvas2, '@', 20, 6, 13, 0);
    ascii::DrawLine(canvas2, '@', 20, 8, 13, 14);

    //std::cout << "0123456789A123456789B123456789C123456789D" << std::endl;
    while (true) {
        for (int i = 0 ; i < 10 ; ++i) {
            std::cout << std::endl << std::endl;
        }
        canvas.Print(std::cout);
        sleep(1);
        for (int i = 0 ; i < 10 ; ++i) {
            std::cout << std::endl << std::endl;
        }
        canvas2.Print(std::cout);
        sleep(1);
    }

    return 0;
}
