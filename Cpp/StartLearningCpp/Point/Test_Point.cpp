#include <cmath>
#include <stdio.h> // printf
#include "Point.hpp"


int main() {
    Point p1(1, 1);
    Point p2(2, 2);

    double checkDistanceFromOrigin = p1.DistanceToOrigin();
    if(sqrt(2) == checkDistanceFromOrigin) {
        printf("Passed!\n");
    }
    else {
        printf("Failed...\n");
    }

    double checkDistanceToOtherPoint = p1.DistanceTo(&p2);
    if(sqrt(2) == checkDistanceToOtherPoint) {
        printf("Passed!\n");
    }
    else {
        printf("Failed...\n");
    }

    Point p3 = p1.Add(&p2);
    p3.Print();
    printf("Should be: (3.0, 3.0)\n");

    Point p4 = p1.Substact(&p2);
    p4.Print();
    printf("Should be: (-1.0, -1.0)\n");

    return 0;
}