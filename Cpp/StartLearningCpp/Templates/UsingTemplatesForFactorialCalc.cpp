// An example of using templates engine to calculate factorial,
// Note: writing the INSTANTIATIONS manually instead of letting the compiler do it by itself (learning purposes)
#include <iostream>

// An example for a Templates:

// template<typename T>
// T Max(T a_a, T a_b) {
//     return (a_a > a_b) ? a_a : a_b;
// }


// // SPECIALIZATION - SPECIFIC SPECIAL CASE IMPLEMENTATION
// template<>
// const char * Max<const char *>(const char * a_a, const char * a_b) {
//     return strcmp(a_a, a_b) >= 0 ? a_a : a_b;
// }




template <long N>
struct Factorial {
    enum { VAL = N * Factorial<N - 1>::VAL};
};

template<>
struct Factorial<0> { // Base case
    enum { VAL = 1 };
};

// -------------------------------------------------------------
// AN EXAMPLE FOR INSTANTIATIONs OF the given TEMPLATE: (WHAT THE COMPILER DOES UNDERNEATH THE HOOD in the compile time)

// for 1:
template<>
struct Factorial<1> {
    enum { VAL = 1 * Factorial<0>::VAL };
};

// for 2:
template<>
struct Factorial<2> {
    enum { VAL = 1 * Factorial<1>::VAL };
};

// for 3:
template<>
struct Factorial<3> {
    enum { VAL = 1 * Factorial<2>::VAL };
};

// for 4:
template<>
struct Factorial<4> {
    enum { VAL = 1 * Factorial<3>::VAL };
};

// for 5:
template<>
struct Factorial<5> {
    enum { VAL = 1 * Factorial<4>::VAL };
};

// for 6:
template<>
struct Factorial<6> {
    enum { VAL = 1 * Factorial<5>::VAL };
};

// for 7:
template<>
struct Factorial<7> {
    enum { VAL = 1 * Factorial<6>::VAL };
};

// --------------------------------------------------------

int main() { // the compiler KNOWS(!) in COMPILE TIME - what is the answer for each expression here! => faster executable (but the compile is generating A LOT of code...)
    std::cout << Factorial<0>::VAL << std::endl; // 0! == 1
    std::cout << Factorial<3>::VAL << std::endl; // 3! == 6
    std::cout << Factorial<5>::VAL << std::endl; // 5! == 120
    std::cout << Factorial<7>::VAL << std::endl; // 7! == 5040

    return 0;
}