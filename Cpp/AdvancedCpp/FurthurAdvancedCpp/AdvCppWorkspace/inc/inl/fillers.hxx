#ifndef NM_FILLERS_HXX
#define NM_FILLERS_HXX


#include <cstddef> // size_t
#include <type_traits> // std::is_arithmetic, std::is_same


namespace nm {

namespace advcpp {

template <typename NumericT, typename NumericU>
void Fill(NumericT* a_collectionToFill, const NumericT a_startingValue, const NumericU a_valueToIncrement, const size_t a_sizeOfCollection) {
    static_assert(std::is_arithmetic<NumericT>::value, "NumericT must be a arithmetic type");
    static_assert(std::is_arithmetic<NumericU>::value, "NumericU must be a arithmetic type");

    NumericT value = a_startingValue;
    for(size_t i = 0; i < a_sizeOfCollection; ++i) {
        a_collectionToFill[i] = value;
        value += a_valueToIncrement;
    }
}


template <typename ContainerT, typename NumericT, typename NumericU>
void Fill(ContainerT& a_containerToFill, const NumericT a_startingValue, const NumericT a_valueToIncrement, const size_t a_sizeOfContainer) {
    static_assert(std::is_arithmetic<NumericT>::value, "NumericT must be a arithmetic type");
    static_assert(std::is_arithmetic<NumericU>::value, "NumericU must be a arithmetic type");
    static_assert(std::is_same<typename ContainerT::value_type, NumericT>::value, "ContainerT must be specialized to type Container<NumericT>");

    NumericT value = a_startingValue;
    for(size_t i = 0; i < a_sizeOfContainer; ++i) {
        a_containerToFill.push_back(value);
        value += a_valueToIncrement;
    }
}


template <typename FwdIteratorT, typename NumericT, typename NumericU>
void Fill(FwdIteratorT a_begin, FwdIteratorT a_end, const NumericT a_startingValue, const NumericU a_valueToIncrement) {
    static_assert(std::is_arithmetic<NumericT>::value, "NumericT must be an arithmetic type");
    static_assert(std::is_arithmetic<NumericU>::value, "NumericU must be a arithmetic type");
    // static_assert(std::is_same<typename FwdIteratorT::value_type, NumericT>::value, "FwdIteratorT::value_type must be of type T"); // would not work with raw pointers

    NumericT value = a_startingValue;
    while(a_begin != a_end) {
        *a_begin = value;
        value += a_valueToIncrement;
        ++a_begin;
    }
}

} // advcpp

} // nm


#endif // NM_FILLERS_HXX