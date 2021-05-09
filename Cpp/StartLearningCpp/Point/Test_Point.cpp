#include <cmath>
#include <iostream> // std::cout, std::endl
#include "Point.hpp"


int main() {
    Point p1(1, 1);
    Point p2(2, 2);

    double checkDistanceFromOrigin = p1.DistanceFromOrigin();
    if(sqrt(2) == checkDistanceFromOrigin) {
        std:: cout << "Passed!" << std::endl;
    }
    else {
        std:: cout << "Failed..." << std::endl;
    }

    double checkDistanceToOtherPoint = p1.DistanceTo(p2);
    if(sqrt(2) == checkDistanceToOtherPoint) {
        std:: cout << "Passed!" << std::endl;
    }
    else {
        std:: cout << "Failed..." << std::endl;
    }

    Point p3 = p1 + p2;
    p3.Print();
    std::cout << "\nShould be: (3, 3)" << std::endl;

    Point p4 = p1 - p2;
    p4.Print();
    std::cout << "\nShould be: (-1, -1)" << std::endl;

    return 0;
}