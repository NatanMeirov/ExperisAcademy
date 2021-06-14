#include <cstdio>
#include "BitsInByteLookupTable.hpp"



int main() {
    constexpr int x1 = nm::meta::BitsInByteLookupTable<3>::Value; // 2
    constexpr int x2 = nm::meta::BitsInByteLookupTable<0>::Value; // 0
    constexpr int x3 = nm::meta::BitsInByteLookupTable<1>::Value; // 1
    constexpr int x4 = nm::meta::BitsInByteLookupTable<255>::Value; // 8
    constexpr int x5 = nm::meta::BitsInByteLookupTable<254>::Value; // 7
    constexpr int x6 = nm::meta::BitsInByteLookupTable<128>::Value; // 1
    constexpr int x7 = nm::meta::BitsInByteLookupTable<127>::Value; // 7

    printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n", x1, x2, x3, x4, x5, x6, x7);

    return 0;
}