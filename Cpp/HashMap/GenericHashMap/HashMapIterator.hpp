#ifndef __HASHMAP_HASHMAPITERATOR_HPP__
#define __HASHMAP_HASHMAPITERATOR_HPP__


#include <iterator>
#include "HashMapTypes.hpp"


namespace HashMap {

template <typename K, typename V>
class HashMapIterator /*: public std::forward_iterator_tag<HashMapIterator<K,V>, >*/ {
public:
    explicit HashMapIterator(typename HashMap::Types<K,V>::OuterContainer::iterator a_outerContainerBeginIterator, typename HashMap::Types<K,V>::OuterContainer::iterator a_outerContainerEndIterator);
    explicit HashMapIterator(typename HashMap::Types<K,V>::OuterContainer::iterator a_outerContainerCurrentIterator, typename HashMap::Types<K,V>::OuterContainer::iterator a_outerContainerEndIterator, typename HashMap::Types<K,V>::InnerContainer::iterator a_innerContainerCurrentIterator);
    HashMapIterator(const HashMapIterator<K,V>& a_other);
    HashMapIterator<K,V>& operator=(const HashMapIterator<K,V>& a_other);
    ~HashMapIterator() = default;

    HashMapIterator<K,V>& operator++();
    V& operator*();
    V operator*() const;
    bool operator!=(const HashMapIterator<K,V>& a_other);
    bool operator==(const HashMapIterator<K,V>& a_other);

private:
    void PlaceIteratorOnNextValidLocation();

    typename HashMap::Types<K,V>::OuterContainer::iterator m_outerContainerCurrentIterator;
    typename HashMap::Types<K,V>::OuterContainer::iterator m_outerContainerEndIterator;
    typename HashMap::Types<K,V>::InnerContainer::iterator m_innerContainerCurrentIterator;
    typename HashMap::Types<K,V>::InnerContainer::iterator m_innerContainerEndIterator;
};

} // HashMap


#endif // __HASHMAP_HASHMAPITERATOR_HPP__