/**
 * @file BitsInByteLookupTable.hpp
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Metaprogramming Bits In Byte Lookup Table implementation
 * @version 1.0
 * @date 2021-06-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NM_META_BITSINBYTELOOKUPTABLE_HPP__
#define __NM_META_BITSINBYTELOOKUPTABLE_HPP__


#include <climits> // CHAR_BIT


namespace nm {

namespace meta {

// Assumption: Byte (unsigned char) has 8 bits (== CHAR_BIT)
// NOTE: To initialize the who;e table - we must make the compiler instantiate BitsInByteTable<255> (and it will trigger the whole table initialization)

template <unsigned char Byte, unsigned char BitLocation>
struct IsOnBit {
    enum { Result = ((Byte & ((unsigned char)1 << (unsigned char)(BitLocation - 1))) ? 1 : 0) };
};


template <unsigned char N, unsigned char CurrentBit >
struct CountOnBits {
    enum { Count = IsOnBit<N, CurrentBit>::Result + CountOnBits<N, CurrentBit - 1>::Count };
};


// Specialization of CountOnBits of the first bit, to end the recursion
template <unsigned char N>
struct CountOnBits<N, 1> {
    enum { Count = IsOnBit<N, 1>::Result };
};


template <unsigned char N>
struct BitsInByteLookupTable {
    enum { Value = CountOnBits<N, 8>::Count };
private:
    enum { NextEntry = BitsInByteLookupTable<N - 1>::Value }; // To calculate the whole table recursively
};


// Specialization of BitsInByteLookupTable of the last byte value (0), to end the recursion
template <>
struct BitsInByteLookupTable<0> {
    enum { Value = 0 };
private:
    enum { NextEntry = 0 };
};

} // meta

} //nm


#endif // __NM_META_BITSINBYTELOOKUPTABLE_HPP__