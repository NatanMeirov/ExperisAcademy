#include "../Bitset.hpp"
#include <iostream> // std::cout, std::endl


void TestAreAllOnAndOff();


int main() {
    TestAreAllOnAndOff();

    return 0;
}


void TestAreAllOnAndOff() {
    NM::Bitset::Bitset<10> bitset;

    for(size_t i = 0; i < 10; ++i) {
        bitset.On(i + 1);
    }

    std::cout << "AreAllOn() Test:" << std::endl;

    if(bitset.AreAllOn()) {
        std::cout << "Passed!" << std::endl;
    }
    else {
        std::cout << "Failed..." << std::endl;
    }

    bitset.Print(std::cout);
    std::cout << std::endl;

    bitset.Off(5);
    if(bitset.AreAllOn()) {
        std::cout << "Failed..." << std::endl;
    }
    else {
        std::cout << "Passed!" << std::endl;
    }

    bitset.Print(std::cout);
    std::cout << std::endl;

    bitset.Toggle(5);
    if(bitset.AreAllOn()) {
        std::cout << "Passed!" << std::endl;
    }
    else {
        std::cout << "Failed..." << std::endl;
    }

    bitset.Print(std::cout);
    std::cout << std::endl;


    for(size_t i = 0; i < 10; ++i) {
        bitset.Off(i + 1);
    }

    std::cout << "AreAllOff() Test:" << std::endl;

    if(bitset.AreAllOff()) {
        std::cout << "Passed!" << std::endl;
    }
    else {
        std::cout << "Failed..." << std::endl;
    }

    bitset.Print(std::cout);
    std::cout << std::endl;

    bitset.Toggle(5);
    if(bitset.AreAllOff()) {
        std::cout << "Failed..." << std::endl;
    }
    else {
        std::cout << "Passed!" << std::endl;
    }

    bitset.Print(std::cout);
    std::cout << std::endl;
}