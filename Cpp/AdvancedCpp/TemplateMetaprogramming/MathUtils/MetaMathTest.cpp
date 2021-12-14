#include <iostream>
#include "Power.hpp"
#include "Factorial.hpp"
#include "Fibonacci.hpp"


int main() {
    std::cout << nm::meta::Factorial<5>::Value << std::endl; // 120
    std::cout << nm::meta::Factorial<6>::Value << std::endl; // 720
    std::cout << nm::meta::Factorial<4>::Value << std::endl; // 24

    std::cout << nm::meta::Power<2,3>::Value << std::endl; // 8
    std::cout << nm::meta::Power<6,2>::Value << std::endl; // 36
    std::cout << nm::meta::Power<2,63>::Value << std::endl; // 9223372036854775808
    std::cout << nm::meta::Power<7,3>::Value << std::endl; // 343

    std::cout << nm::meta::Fibonacci<2>::Value << std::endl; // 1
    std::cout << nm::meta::Fibonacci<20>::Value << std::endl; // 6765
    std::cout << nm::meta::Fibonacci<7>::Value << std::endl; // 8
    std::cout << nm::meta::Fibonacci<10>::Value << std::endl; // 34
    std::cout << nm::meta::Fibonacci<19>::Value << std::endl; // 4181


    return 0;
}