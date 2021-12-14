#ifndef __NM_BITSET_BITSET_HPP__
#define __NM_BITSET_BITSET_HPP__


#include <ostream> // ostream
#include <cstddef> // size_t
#include <cstring> // memcpy, memset
#include <climits> // CHAR_BIT
#include <algorithm>
#include <numeric> // accumulate
#include <stdexcept> // std::out_of_range
#include "BitsetTypes.hpp"
#include "BitsOperations.hpp"
#include "BitProxy.hpp"


namespace NM {

namespace Bitset {

inline constexpr Types::BiggestUnsignedNumberType DivideAndCeil(Types::BiggestUnsignedNumberType a_numerator, Types::BiggestUnsignedNumberType a_denominator) {
    return a_numerator / a_denominator + ((a_numerator % a_denominator == 0) ? 0 : 1);
}


template<Types::BiggestUnsignedNumberType N>
class Bitset {
private:
    typedef Types::BiggestUnsignedNumberType BitsetUnit;
    static constexpr unsigned short BITS_IN_BITSET_UNIT = sizeof(BitsetUnit) * CHAR_BIT;
    static constexpr Types::BiggestUnsignedNumberType BITSET_UNITS_COUNT = DivideAndCeil(N, BITS_IN_BITSET_UNIT);

    template<Types::BiggestUnsignedNumberType M>
    class BitsetArray {
    public:
        BitsetArray() : m_bitsetUnitsArray() {}
        BitsetArray(const BitsetArray& a_other) = default; // Copy C'tor knows that it should use memcpy for us
        BitsetArray& operator=(const BitsetArray& a_other) = default; // The compiler uses memcpy for us
        ~BitsetArray() = default;

        const BitsetUnit* Begin() const {return this->m_bitsetUnitsArray; }
        const BitsetUnit* End() const {return this->m_bitsetUnitsArray + M; } // Points to the address AFTER the array's end
        BitsetUnit* Begin() {return this->m_bitsetUnitsArray; }
        BitsetUnit* End() {return this->m_bitsetUnitsArray + M; } // Points to the address AFTER the array's end

    private:
        BitsetUnit m_bitsetUnitsArray[M];
    };

public:
    typedef BitsetUnit* Iterator; // operator++(), operator*() and operator!=() - already built-in while using a raw pointer

    Bitset() : m_bitset() {}
    Bitset(const Bitset& a_other) = default;
    Bitset& operator=(const Bitset& a_other) = default;
    ~Bitset() = default;

    // Iterator Begin() const;
    // Iterator End() const;
    Types::BiggestUnsignedNumberType Size() const;
    void Print(std::ostream& a_os) const;

    // Bits operations:

    // In-place (modifiable)
    void On(const Types::BiggestUnsignedNumberType a_bit); // Throws std::out_of_range
    void Off(const Types::BiggestUnsignedNumberType a_bit); // Throws std::out_of_range
    void Toggle(const Types::BiggestUnsignedNumberType a_bit); // Throws std::out_of_range
    Bitset<N>& operator~();
    Bitset<N>& operator&=(const Bitset<N>& a_other);
    Bitset<N>& operator|=(const Bitset<N>& a_other);
    Bitset<N>& operator^=(const Bitset<N>& a_other);
    BitProxy operator[](const Types::BiggestUnsignedNumberType a_bit);

    // Not In-place (not modifiable)
    Types::BiggestUnsignedNumberType CountOn() const;
    Types::BiggestUnsignedNumberType CountOff() const;
    bool AreAllOn() const;
    bool AreAllOff() const;
    Bitset<N> operator|(const Bitset<N>& a_other) const;
    Bitset<N> operator&(const Bitset<N>& a_other) const;
    Bitset<N> operator^(const Bitset<N>& a_other) const;
    bool operator==(const Bitset<N>& a_other) const;
    bool operator!=(const Bitset<N>& a_other) const;


private:
    template <typename FunctorT>
    void OperateOnBit(const Types::BiggestUnsignedNumberType a_bit, FunctorT a_functor);
    template <typename ReturnT, typename FunctorT>
    ReturnT OperateOnBitsetUnit(const BitsetUnit* a_bitsetUnit,  FunctorT a_functor) const;
    bool HasAReminder() const;
    unsigned short GetValidReminderBits() const;
    void ClearInvalidReminderBits();
    BitsetUnit* GetBitsetUnitOfBit(const Types::BiggestUnsignedNumberType a_bit);
    unsigned short GetBitIndexInBitsetUnit(const Types::BiggestUnsignedNumberType a_bit);

    BitsetArray<BITSET_UNITS_COUNT> m_bitset;
};


// Bitset_Inline:


template <Types::BiggestUnsignedNumberType N>
template <typename FunctorT>
void Bitset<N>::OperateOnBit(const Types::BiggestUnsignedNumberType a_bit, FunctorT a_functor) {
    Bitset::BitsetUnit* unitOfBit = this->GetBitsetUnitOfBit(a_bit);
    Types::BiggestUnsignedNumberType bitN = this->GetBitIndexInBitsetUnit(a_bit);
    a_functor(*unitOfBit, bitN);
}


template <Types::BiggestUnsignedNumberType N>
template <typename ReturnT, typename FunctorT>
ReturnT Bitset<N>::OperateOnBitsetUnit(const BitsetUnit* a_bitsetUnit, FunctorT a_functor) const {
    return a_functor(const_cast<BitsetUnit&>(*a_bitsetUnit));
}


template<Types::BiggestUnsignedNumberType N>
typename Bitset<N>::BitsetUnit* Bitset<N>::GetBitsetUnitOfBit(const Types::BiggestUnsignedNumberType a_bit) {
    return this->m_bitset.Begin() + (a_bit / Bitset::BITS_IN_BITSET_UNIT); // The index is rounded, so the answer is correct
}


template<Types::BiggestUnsignedNumberType N>
unsigned short Bitset<N>::GetBitIndexInBitsetUnit(const Types::BiggestUnsignedNumberType a_bit) {
    return a_bit == Bitset::BITS_IN_BITSET_UNIT ? Bitset::BITS_IN_BITSET_UNIT : a_bit % Bitset::BITS_IN_BITSET_UNIT;
}


template<Types::BiggestUnsignedNumberType N>
bool Bitset<N>::HasAReminder() const {
    return N % Bitset::BITS_IN_BITSET_UNIT != 0;
}


template<Types::BiggestUnsignedNumberType N>
unsigned short Bitset<N>::GetValidReminderBits() const {
    return N % Bitset::BITS_IN_BITSET_UNIT;
}


template<Types::BiggestUnsignedNumberType N>
void Bitset<N>::ClearInvalidReminderBits() {
    this->OperateOnBitsetUnit<void>(this->m_bitset.End() - 1, [](){});
}


// template<Types::BiggestUnsignedNumberType N>
// typename Bitset<N>::Iterator Bitset<N>::Begin() const {
//     return this->m_bitset.Begin();
// }


// template<Types::BiggestUnsignedNumberType N>
// typename Bitset<N>::Iterator Bitset<N>::End() const {
//     return this->m_bitset.End();
// }


template<Types::BiggestUnsignedNumberType N>
Types::BiggestUnsignedNumberType Bitset<N>::Size() const {
    return N;
}


template<Types::BiggestUnsignedNumberType N>
void Bitset<N>::Print(std::ostream& a_os) const {
    Types::BiggestUnsignedNumberType validBits = N;
    a_os << "{\n";
    std::for_each(this->m_bitset.Begin(), this->m_bitset.End(), [&a_os, &validBits](Bitset::BitsetUnit a_bitsetUnit){
        Types::BiggestUnsignedNumberType mask = Types::BiggestUnsignedNumberType(1);
        a_os << "  ([ ";
        for(size_t i = 0; i < Bitset::BITS_IN_BITSET_UNIT && validBits > 0; ++i) {
            if(i % CHAR_BIT == 0 && i != 0) {
                a_os << "]\n   [ ";
            }

            a_os << (((a_bitsetUnit & mask) != 0) ? 1 : 0) << " ";
            mask <<= 1;
            --validBits;
        }
        a_os << "])\n";
    });
    a_os << "}";
}


template<Types::BiggestUnsignedNumberType N>
void Bitset<N>::On(const Types::BiggestUnsignedNumberType a_bit) {
    if(a_bit > N || a_bit == 0) {
        throw std::out_of_range("Out Of Bounds Error");
    }

    this->OperateOnBit(a_bit, [](Bitset::BitsetUnit& a_unitOfBit, const Types::BiggestUnsignedNumberType a_bitN){ TurnNBitOn(a_unitOfBit, a_bitN); });
}


template<Types::BiggestUnsignedNumberType N>
void Bitset<N>::Off(const Types::BiggestUnsignedNumberType a_bit) {
    if(a_bit > N || a_bit == 0) {
        throw std::out_of_range("Out Of Bounds Error");
    }

    this->OperateOnBit(a_bit, [](Bitset::BitsetUnit& a_unitOfBit, const Types::BiggestUnsignedNumberType a_bitN){ TurnNBitOff(a_unitOfBit, a_bitN); });
}


template<Types::BiggestUnsignedNumberType N>
void Bitset<N>::Toggle(const Types::BiggestUnsignedNumberType a_bit) {
    if(a_bit > N || a_bit == 0) {
        throw std::out_of_range("Out Of Bounds Error");
    }

    this->OperateOnBit(a_bit, [](Bitset::BitsetUnit& a_unitOfBit, const Types::BiggestUnsignedNumberType a_bitN){ ToggleNBit(a_unitOfBit, a_bitN); });
}


template<Types::BiggestUnsignedNumberType N>
Types::BiggestUnsignedNumberType Bitset<N>::CountOn() const {
    Types::BiggestUnsignedNumberType bitsToCount = Bitset::BITS_IN_BITSET_UNIT;
    return std::accumulate(this->m_bitset.Begin(), this->m_bitset.End(), 0, [&bitsToCount](Types::BiggestUnsignedNumberType a_totalCountedOnBits, Bitset::BitsetUnit a_bitsetUnit){ return a_totalCountedOnBits + CountOnBitsOfRange(a_bitsetUnit, bitsToCount); });
}


template<Types::BiggestUnsignedNumberType N>
Types::BiggestUnsignedNumberType Bitset<N>::CountOff() const {
    return N - this->CountOn();
}


template<Types::BiggestUnsignedNumberType N>
bool Bitset<N>::AreAllOn() const {
    if(!this->HasAReminder()) {
        return std::all_of(this->m_bitset.Begin(), this->m_bitset.End(), [](const Bitset::BitsetUnit& a_bitsetUnit){ return a_bitsetUnit == ~(Bitset::BitsetUnit(0)); }); // Convert to BitsetUnit type and transform 00..00 to 11..11
    }

    unsigned short validReminderBits = this->GetValidReminderBits();
    return std::all_of(this->m_bitset.Begin(), this->m_bitset.End() - 1, [](const Bitset::BitsetUnit& a_bitsetUnit){ return a_bitsetUnit == ~(Bitset::BitsetUnit(0)); })
    && this->OperateOnBitsetUnit<bool>((this->m_bitset.End() - 1), [&validReminderBits](const Bitset::BitsetUnit& a_bitsetUnit){
        BitsetUnit mask = ((BitsetUnit(~0)) << validReminderBits);
        return (~mask) == a_bitsetUnit;
    });
}


template<Types::BiggestUnsignedNumberType N>
bool Bitset<N>::AreAllOff() const {
    return std::all_of(this->m_bitset.Begin(), this->m_bitset.End(), [](const Bitset::BitsetUnit& a_bitsetUnit){ return a_bitsetUnit == Bitset::BitsetUnit(0); });
}


// TODO (lack of time):

template<Types::BiggestUnsignedNumberType N>
Bitset<N>& Bitset<N>::operator~() {}


template<Types::BiggestUnsignedNumberType N>
Bitset<N>& Bitset<N>::operator&=(const Bitset<N>& a_other) {}


template<Types::BiggestUnsignedNumberType N>
Bitset<N>& Bitset<N>::operator|=(const Bitset<N>& a_other) {}


template<Types::BiggestUnsignedNumberType N>
Bitset<N>& Bitset<N>::operator^=(const Bitset<N>& a_other) {}


template<Types::BiggestUnsignedNumberType N>
BitProxy Bitset<N>::operator[](const Types::BiggestUnsignedNumberType a_bit) {
    BitsetUnit* bitsetUnit = this->GetBitsetUnitOfBit(a_bit);
    unsigned short bitIndex = this->GetBitIndexInBitsetUnit(a_bit);

    return BitProxy(*bitsetUnit, bitIndex);
}


template<Types::BiggestUnsignedNumberType N>
Bitset<N> Bitset<N>::operator|(const Bitset<N>& a_other) const {} // use |=


template<Types::BiggestUnsignedNumberType N>
Bitset<N> Bitset<N>::operator&(const Bitset<N>& a_other) const {} // use &=


template<Types::BiggestUnsignedNumberType N>
Bitset<N> Bitset<N>::operator^(const Bitset<N>& a_other) const {} // use ^=


template<Types::BiggestUnsignedNumberType N>
bool Bitset<N>::operator==(const Bitset<N>& a_other) const {} // Use std::equal() function


template<Types::BiggestUnsignedNumberType N>
bool Bitset<N>::operator!=(const Bitset<N>& a_other) const {}


} // Bitset

} // NM


#endif // __NM_BITSET_BITSET_HPP__