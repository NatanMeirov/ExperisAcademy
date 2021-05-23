#ifndef __HASHMAP_HASHMAPITERATOR_HPP__
#define __HASHMAP_HASHMAPITERATOR_HPP__


#include <vector>
#include <list>
#include "KeyValuePair_Inline.hpp"


namespace HashMap {

template <typename K, typename V>
class HashMapIterator {
public:
    HashMapIterator(typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorBeginIterator, typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorEndIterator);
    HashMapIterator(typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorCurrentIterator, typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorEndIterator, typename std::list<KeyValuePair<K,V>>::iterator a_listCurrentIterator);
    HashMapIterator(const HashMapIterator<K,V>& a_other) = default;
    HashMapIterator<K,V>& operator=(const HashMapIterator<K,V>& a_other) = default;
    ~HashMapIterator() = default;

    HashMapIterator<K,V>& operator++();
    // HashMapIterator<K,V>& operator--(); // TODO
    V& operator*();
    V operator*() const;
    bool operator!=(const HashMapIterator<K,V>& a_other);
    // bool operator==(const HashMapIterator<K,V>& a_other); // TODO

private:
    void PlaceIteratorOnNextValidLocation(bool a_isInitializationPart = false);
    // void PlaceIteratorOnPreviousValidLocation(); // TODO

    typename std::vector<std::list<KeyValuePair<K,V>>>::iterator m_vectorCurrentIterator;
    typename std::vector<std::list<KeyValuePair<K,V>>>::iterator m_vectorEndIterator;
    typename std::list<KeyValuePair<K,V>>::iterator m_listCurrentIterator;
    typename std::list<KeyValuePair<K,V>>::iterator m_listEndIterator;
};

} // HashMap


#endif // __HASHMAP_HASHMAPITERATOR_HPP__