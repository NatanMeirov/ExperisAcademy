#ifndef NM_SEARCH_ALG_HXX
#define NM_SEARCH_ALG_HXX


#include <cstddef> // size_t
// #include <type_traits> // std::is_same
#include <functional> // std::less


namespace nm {

namespace advcpp {

template <typename ContainerT, typename T>
size_t SearchAlgorithms::BinarySearch(const T& a_key, const ContainerT& a_container, const size_t a_sizeOfContainer) {
    // static_assert(std::is_same<typename ContainerT::value_type, T>::value, "ContainerT must be specialized to type Container<T>"); // Static assertion does not work on <int*, int>...

    size_t head = 0;
    size_t tail = a_sizeOfContainer - 1;
    while(head <= tail) {
        size_t middle = size_t(head + ((tail - head) / 2));

        if(std::less<T>()(a_container[middle], a_key)) {
            head = middle + 1;
        }
        else if(std::less<T>()(a_key, a_container[middle])) {
            if(middle == 0) { // To avoid the case of head == tail == 0 -> size_t would be -1 == MAX_SIZE_T
                break;
            }

            tail = middle - 1;
        }
        else { // a_container[middle] == a_key
            return size_t(middle);
        }
    }

    return SearchAlgorithms::MAX_SIZE_T; // Key not found
}


template <typename RandomAccessIteratorT, typename T>
RandomAccessIteratorT BinarySearch(const T& a_key, RandomAccessIteratorT a_begin, RandomAccessIteratorT a_end) {
    RandomAccessIteratorT head = a_begin;
    RandomAccessIteratorT tail = a_end;

    while(head <= tail) {
        RandomAccessIteratorT middle = (head + ((tail - head) / 2));

        if(std::less<T>()(*middle, a_key)) {
            head = middle + 1;
        }
        else if(std::less<T>()(a_key, *middle)) {
            if(middle == a_begin) {
                break;
            }

            tail = middle - 1;
        }
        else {
            return middle;
        }
    }

    return a_end; // Key not found
}


} // advcpp

} // nm


#endif // NM_SEARCH_ALG_HXX