#ifndef __HASHMAP_HASHMAP_HPP__
#define __HASHMAP_HASHMAP_HPP__


#include <cstddef> // size_t
#include <vector>
#include <list>
#include "KeyValuePair.hpp"
#include "KeyHashingHandler.hpp"
#include "HashMapIterator.hpp"


namespace HashMap {

// Note: The key-value pairs are stored by value
// Note: Deallocation is the user responsibility (if required)
// K concept: operator== (equality)
template<typename K, typename V>
class HashMap {
public:
    typedef HashMapIterator<K,V> Iterator;

    HashMap(const size_t a_initialSize, const KeyHashingHandler<K>* a_hashHandler); // InitialSize MUST be >= 1, throws std::invalid_argument
    HashMap(const HashMap<K,V>& a_map) = default; //! Warning - a shallow copy would occur to the pointer of KeyHashingHandler on copy
    HashMap<K,V>& operator=(const HashMap<K,V>& a_map) = default; //! Same as above
    ~HashMap() = default;

    // V& operator[](const K& a_key); TODO
    // V operator[](const K& a_key) const; TODO
    void Insert(const K& a_key, const V& a_value);
    void Remove(const K& a_key);
    Iterator Find(const K& a_key) const; // Returns an iterator to the Value of exists, else an Iterator that points to the end of the HashMap
    size_t Size() const; // Time-Complexity: O(1)
    Iterator Begin() const;
    Iterator End() const;

private:
    typename std::list<KeyValuePair<K,V>>::iterator SearchByKey(const K& a_key) const;

    std::vector<std::list<KeyValuePair<K,V>>> m_buckets;
    KeyHashingHandler<K>* m_hashHandler;
    size_t m_totalKeyValuePairs;
};

} // HashMap


#endif // __HASHMAP_HASHMAP_HPP__