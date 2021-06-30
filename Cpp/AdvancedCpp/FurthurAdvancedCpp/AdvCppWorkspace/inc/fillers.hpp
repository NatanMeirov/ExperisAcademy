#ifndef NM_FILLERS_HPP
#define NM_FILLERS_HPP


#include <cstddef> // size_t

namespace nm {

namespace advcpp {

// Concept of NumericT: must be copy-constructable, must be an arithmetic type
// Concept of NumericU: must be an arithmetic type
template <typename NumericT, typename NumericU>
void Fill(NumericT* a_collectionToFill, const NumericT a_startingValue, const NumericU a_valueToIncrement, const size_t a_sizeOfCollection);


// Concept of ContainetT: must implement push_back()
// Concept of NumericT: must be copy-constructable, must be an arithmetic type
// Concept of NumericU: must be an arithmetic type
template <typename ContainerT, typename NumericT, typename NumericU>
void Fill(ContainerT& a_containerToFill, const NumericT a_startingValue, const NumericU a_valueToIncrement, const size_t a_sizeOfContainer);


// Concept of FwdIteratorT: must be a forward iterator tag
// Concept of NumericT: must be copy-constructable, must be an arithmetic type
// Concept of NumericU: must be an arithmetic type
template <typename FwdIteratorT, typename NumericT, typename NumericU>
void Fill(FwdIteratorT a_begin, FwdIteratorT a_end, const NumericT a_startingValue, const NumericU a_valueToIncrement);

} // advcpp

} // nm


#include "inl/fillers.hxx"


#endif // NM_FILLERS_HPP