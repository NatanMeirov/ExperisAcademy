// inline functions - the linking symbols of the functions are Weak (can be copied - and it will not cause a multiple definitions fault [W symbol tells the linker that he can pick one definition randomly - all are the same EVEN if there are multiple definitions of one function])
#ifndef __NM_BITSET_BITSOPERATIONS_HPP__
#define __NM_BITSET_BITSOPERATIONS_HPP__


#include "BitsetTypes.hpp"


namespace NM {

namespace Bitset {

template <typename T>
inline bool CheckNBit(const T& a_bitsetUnit, const unsigned short a_bit) {
    return a_bitsetUnit & (T(1) << (a_bit - 1));
}


template <typename T>
inline void TurnNBitOn(T& a_bitsetUnit, const unsigned short a_bit) {
    a_bitsetUnit |= (T(1) << (a_bit - 1));
}


template <typename T>
inline void TurnNBitOff(T& a_bitsetUnit, const unsigned short a_bit) {
    a_bitsetUnit &= ~(T(1) << (a_bit - 1)); // Convert to T
}


template <typename T>
inline void ToggleNBit(T& a_bitsetUnit, const unsigned short a_bit) {
    a_bitsetUnit ^= (T(1) << (a_bit - 1)); // Convert to T
}


template <typename T>
inline unsigned short CountOnBitsOfRange(const T& a_bitsetUnit, unsigned short a_lastBitToCount) {
    unsigned short counter = 0;
    T bitsetUnitValue = a_bitsetUnit;
    while(bitsetUnitValue > 0 && a_lastBitToCount > 0) {
        counter += bitsetUnitValue & T(1); // Convert to T
        bitsetUnitValue >>= 1;
        --a_lastBitToCount;
    }

    return counter;
}


template <typename T>
inline void SetRangeOfBitsToValue(T& a_bitsetUnit, unsigned short a_lastBitToSet, bool a_binaryValueToSet) { // 0 for false, 1 for true
    T mask = ((~T(0)) << a_lastBitToSet);
    if(a_binaryValueToSet) { // Set to 1
        a_bitsetUnit |= (~mask);
    }
    else { // Set to 0
        a_bitsetUnit &= mask;
    }
}

} // Bitset

} // NM


#endif // __NM_BITSET_BITSOPERATION_HPP__