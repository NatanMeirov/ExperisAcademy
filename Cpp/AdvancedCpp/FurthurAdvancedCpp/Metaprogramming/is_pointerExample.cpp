#include <iostream> // std::cout, std::endl


// These type traits are working because the compiler is ALWAYS looking for the EXACT FIT (BEST FIT) for every Template instantiation! so specialization of things are DIRECTING the compiler to the best fit - and can be used JUST LIKE IF-ELSE STATEMENTS IN COMPILE TIME AND NOT IN RUN-TIME!!!!!!


template <typename T>
struct IsPointerType { // General type template
    // enum { result = false };
    static const bool result = false;
};


template <typename T>
struct IsPointerType<T*> { // Special type template (a specialization - to specify the T* type! and we can do it)
    // enum { result = true };
    static const bool result = true;
};


template <typename T>
constexpr bool IsPointer(T a_argumentToCheck) {
    return IsPointerType<T>::result;
}


int main() {
    int x;
    int* px = &x;

    std::cout << "Is a Pointer? " << IsPointer(x) << std::endl;
    std::cout << "Is a Pointer? " << IsPointer(px) << std::endl;
}