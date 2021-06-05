#include <exception>
#include <iostream> // std::cout, std::endl
#include "SharedPointer.hpp"
#include "SharedPointer_Inline.hpp"


namespace nmsp = nm::smartpointers;


int main() {
    nmsp::SharedPointer<int> intPtr = nmsp::SharedPointer<int>::MakeSharedPointer(10);
    std::cout << *intPtr << std::endl;
    *intPtr = 20;
    std::cout << *intPtr << std::endl;

    nmsp::SharedPointer<int> nullIntPtr = nmsp::SharedPointer<int>::MakeSharedPointer(nullptr);
    std::cout << (nullIntPtr == nullptr) << std::endl; // 1 (true)

    nullIntPtr = intPtr;
    std::cout << *nullIntPtr << std::endl; // 20

    std::cout << intPtr.GetSharedReferencesCount() << std::endl; // 2

    nullIntPtr = nullptr;
    std::cout << nullIntPtr.GetSharedReferencesCount() << std::endl; // 0
    try {
        *nullIntPtr = 100;
    }
    catch(const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }

    std::cout << intPtr << std::endl; // A valid address
    std::cout << nullIntPtr << std::endl; // 0 (nullptr)

    // delete nullIntPtr; - won't work

    return 0;
}