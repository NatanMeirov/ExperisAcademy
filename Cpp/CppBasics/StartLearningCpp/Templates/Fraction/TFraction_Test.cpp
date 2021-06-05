#include "TFraction.hpp"
#include "TFraction_Inline.hpp"
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
    std::cout << std::endl;
    std::cout << f1 << std::endl;
    std::cout << (double)f1 << std::endl;

    Fraction<double> f3(2.7, 3.4);
    Fraction<double> f4(f3);
    (f3 * 3).Print();
    std::cout << std::endl;
    f3 + f4;
    f3 - f4;

    // TODO: still not supporting - problem with the casting from Fraction<int> to double (BUG)
    // Fraction<Fraction<int> > f5(f1, f2);
    // std::cout << f5 << std::endl;
}