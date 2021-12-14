#ifndef NM_EXTREMES_HPP
#define NM_EXTREMES_HPP


#include <tuple>


namespace nm
{

namespace advcpp
{

// Time-Complexity: O(3/2*n) => O(n)
// Concept of C: C must be a container that its C::value_type is of type T, and must have C::iterator (forward iterator tag)
// Concept of T: T must be able to be used with std::less (implement operator<)
// Returns std::tuple<nullptr, nullptr> if container is empty
template <typename C, typename T>
std::tuple<T*, T*> FindMinMax(const C& a_container);


// Time-Complexity: O(3/2*n) => O(n)
// Concept of FwdIterator: FwdIterator must be a forward iterator tag
// Returns std::tuple<end(), end()> if container is empty
template <typename FwdIterator>
std::tuple<FwdIterator, FwdIterator> FindMinMax(FwdIterator a_begin, FwdIterator a_end);

} // advcpp

} // nm


#include "inl/extremes.hxx"


#endif // NM_EXTREMES_HPP