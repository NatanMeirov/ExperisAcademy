#ifndef NM_SEARCH_ALG_HPP
#define NM_SEARCH_ALG_HPP


#include <cstddef> // size_t


namespace nm {

namespace advcpp {

// The BinarySearch algorithm returns the index of the key in the container, else returns MAX_SIZE_T or end() iterator (if key now found) [and NOT throwing an exception if key not found]
// Important Note: In order to use the BinarySearch algorithm - the container MUST be sorted!
class SearchAlgorithms {
public:
    // Concept of T: T must be less-compareable [Must implement: std::less]
    // Concept of ContainerT: ContainerT must be a random-accessable container [Must implement: operator[]]
    // Time-Complexity: O(log n)
    // Space-Complexity: O(1)
    // Note: ContainerT must be of type ContainerT<T>
    template <typename ContainerT, typename T>
    static size_t BinarySearch(const T& a_key, const ContainerT& a_container, const size_t a_sizeOfContainer); // returns MAX_SIZE_T if key not found


    // Concept of T: T must be less-compareable [Must implement: std::less]
    // Concept of RandomAccessIteratorT: RandomAccessIteratorT must be a random-access-iterator tag
    // Time-Complexity: O(log n)
    // Space-Complexity: O(1)
    // Note: RandomAccessIteratorT::value_type must be of type T
    template <typename RandomAccessIteratorT, typename T>
    static RandomAccessIteratorT BinarySearch(const T& a_key, RandomAccessIteratorT a_begin, RandomAccessIteratorT a_end); // Returns end() if key not found

    static constexpr size_t MAX_SIZE_T = -1; // == 18446744073709551615
};

} // advcpp

} // nm


#include "inl/seach_alg.hxx"


#endif // NM_SEARCH_ALG_HPP
