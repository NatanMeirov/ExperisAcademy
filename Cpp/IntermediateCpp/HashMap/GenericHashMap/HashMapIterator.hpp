#ifndef __HASHMAP_HASHMAPITERATOR_HPP__
#define __HASHMAP_HASHMAPITERATOR_HPP__


#include <iterator>
#include "HashMapTypes.hpp"

namespace HashMap {

// The HashMapIterator is defined as a Stable Iterator
template <typename K, typename V>
class HashMapIterator /*: public std::forward_iterator_tag<HashMapIterator<K,V>, >*/ {
public:
    typedef typename HashMap::Types<K,V>::OuterContainer::iterator OuterContainerIterator;
    typedef typename HashMap::Types<K,V>::InnerContainer::iterator InnerContainerIterator;

    explicit HashMapIterator(OuterContainerIterator a_outerContainerBeginIterator, OuterContainerIterator a_outerContainerEndIterator);
    explicit HashMapIterator(OuterContainerIterator a_outerContainerCurrentIterator, OuterContainerIterator a_outerContainerEndIterator, InnerContainerIterator a_innerContainerCurrentIterator, const size_t a_offsetFromCurrentToBeginOfOuterContainerIterator); // DO NOT use with End of OuterContainter
    HashMapIterator(const HashMapIterator<K,V>& a_other);
    HashMapIterator<K,V>& operator=(const HashMapIterator<K,V>& a_other);
    ~HashMapIterator() = default;

    HashMapIterator<K,V>& operator++();
    HashMapIterator<K,V>& operator--();
    HashMapIterator<K,V>& operator+=(const size_t a_offset); // Time-Complexity: O(n), If a_offset is more then the distance from the current location to the End - the iterator stops at the End
    HashMapIterator<K,V>& operator-=(const size_t a_offset); // Time-Complexity: O(n), If a_offset is more then the distance from the current location to the Begin - the iterator stops at the Begin
    HashMapIterator<K,V> operator+(const size_t a_offset); // Time-Complexity: O(n), If a_offset is more then the distance from the current location to the End - the iterator stops at the End
    HashMapIterator<K,V> operator-(const size_t a_offset); // Time-Complexity: O(n), If a_offset is more then the distance from the current location to the Begin - the iterator stops at the Begin
    V& operator*() const;
    bool operator!=(const HashMapIterator<K,V>& a_other) const;
    bool operator==(const HashMapIterator<K,V>& a_other) const;

private:
    void PlaceIteratorOnNextValidLocation();
    void EnsureIteratorLocationIsValidOrFindNextValid();
    void PlaceIteratorOnPreviousValidLocation();
    bool IsBegin() const;
    bool IsEnd() const;

    OuterContainerIterator m_outerContainerCurrentIterator;
    OuterContainerIterator m_outerContainerEndIterator;
    InnerContainerIterator m_innerContainerCurrentIterator;
    InnerContainerIterator m_innerContainerEndIterator;
    size_t m_offsetFromCurrentToBeginOfOuterContainerIterator;
};

} // HashMap


#endif // __HASHMAP_HASHMAPITERATOR_HPP__