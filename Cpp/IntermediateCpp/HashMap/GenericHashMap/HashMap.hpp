#ifndef __HASHMAP_HASHMAP_HPP__
#define __HASHMAP_HASHMAP_HPP__


#include <cstddef> // size_t
#include "HashMapTypes.hpp"
#include "HashMapIterator.hpp"
#include "KeyHashingHandler.hpp"


namespace HashMap {

// Note: The key-value pairs are stored by value
// Note: Deallocation is the user responsibility (if required)
// K concept: operator== (equality)
template<typename K, typename V>
class HashMap {
public:
    typedef HashMapIterator<K,V> Iterator;

    HashMap(const size_t a_initialSize, const KeyHashingHandler<K>& a_hashHandler); // InitialSize MUST be >= 1, throws std::invalid_argument
    HashMap(const HashMap<K,V>& a_map) = default;
    HashMap<K,V>& operator=(const HashMap<K,V>& a_map) = default;
    ~HashMap() = default;

    // V& operator[](const K& a_key); TODO
    // V operator[](const K& a_key) const; TODO
    bool Insert(const K& a_key, const V& a_value); // true - on success / false - on failure (Key Already Found in the HashMap)
    bool Remove(const K& a_key); // true - on success / false - on failure (Key Not Found in the HashMap)
    Iterator Find(const K& a_key) const; // Returns an iterator to the Value of exists, else an Iterator that points to the end of the HashMap
    size_t Size() const; // Time-Complexity: O(1)
    Iterator Begin() const;
    Iterator End() const;

private:
    typename Types<K,V>::InnerContainer::iterator SearchByKey(const size_t a_bucketNumber, const K& a_key) const;

    // Members
    typename Types<K,V>::OuterContainer m_buckets;
    const KeyHashingHandler<K>& m_hashHandler;
    size_t m_totalKeyValuePairs;
};

} // HashMap


#endif // __HASHMAP_HASHMAP_HPP__