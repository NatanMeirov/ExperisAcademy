#ifndef __TSWAP_HPP__
#define __TSWAP_HPP__


// A generic swap function implementation
// The CONCEPTS of T is:
// - Copy C'tor
// - Copy-Assignment operator (=)
template <typename T>
void Swap(T& a_firstItem, T& a_secondItem) {
    T swapHelperItem = a_firstItem; // Using the Copy C'tor (for the first initialization)
    a_firstItem = a_secondItem; // Using Copy-Assignment operator
    a_secondItem = swapHelperItem; // Using Copy-Assignment operator
}


#endif // __TSWAP_HPP__