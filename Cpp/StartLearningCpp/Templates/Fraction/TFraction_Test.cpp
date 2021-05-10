#include "TFraction.hpp"
#include <iostream> // std::cout, std::endl



int main() {
    Fraction<int> f1(1, 3);
    Fraction<int> f2(13, 5);
    f1 + f2;
    f1 - f2;
    f1 += f2;
    f1 -= f2;
    f1 * 3;
    f1 * 3.3;
    3 * f1;
    3.6 * f1;
    f1.Print();
    std::cout << f1;
    std::cout << (double)f1;
    Fraction<double> f3(2.7, 3.4);
    Fraction<double> f4(f3);
    f3 * 3;
    f3 + f4;
    f3 - f4;

    Fraction<Fraction<int>> f5(f1, f2);
    std::cout << f5;
}