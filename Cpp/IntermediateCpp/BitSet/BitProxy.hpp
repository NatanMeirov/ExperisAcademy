#ifndef __NM_BITSET_BITPROXY_HPP__
#define __NM_BITSET_BITPROXY_HPP__


#include "BitsOperations.hpp"
#include "BitsetTypes.hpp"


namespace NM {

namespace Bitset {

class BitProxy {
public:
    typedef Types::BiggestUnsignedNumberType BitsCollection;

    explicit BitProxy(BitsCollection& a_bitsCollection, const unsigned short a_currentreferredBit) : m_bitsCollection(a_bitsCollection), m_referredBit(a_currentreferredBit) {}
    BitProxy(const BitProxy& a_other) = default;
    ~BitProxy() = default;

    BitProxy& operator=(const bool a_bitState) { if(a_bitState) { TurnNBitOn(this->m_bitsCollection, this->m_referredBit); } else { TurnNBitOff(this->m_bitsCollection, this->m_referredBit); } return *this; }
    operator bool() { return CheckNBit(this->m_bitsCollection, this->m_referredBit); };
    bool operator==(const BitProxy& a_other) { return CheckNBit(this->m_bitsCollection, this->m_referredBit) ==  CheckNBit(a_other.m_bitsCollection, a_other.m_referredBit); }
    bool operator!=(const BitProxy& a_other) { return CheckNBit(this->m_bitsCollection, this->m_referredBit) !=  CheckNBit(a_other.m_bitsCollection, a_other.m_referredBit); }

private:
    BitsCollection& m_bitsCollection;
    unsigned short m_referredBit; // From 1 to sizeof(Types::BiggestUnsignedNumberType) * CHAR_BITS
};

} // Bitset

} // NM


#endif // __NM_BITSET_BITPROXY_HPP__