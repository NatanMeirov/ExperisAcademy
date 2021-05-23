#ifndef __HASHMAP_HASHMAP_INLINE_HPP__
#define __HASHMAP_HASHMAP_INLINE_HPP__


#include "HashMap.hpp"
#include <cstddef> // size_t
#include <stdexcept> // std::invalid_argument
#include <vector>
#include <list>
#include "KeyValuePair.hpp"
#include "KeyHashingHandler.hpp"
#include "HashMapIterator.hpp"


template <typename K, typename V>
HashMap::HashMap<K,V>::HashMap(const size_t a_initialSize, const KeyHashingHandler<K>* a_hashHandler)
: m_buckets(a_initialSize)
, m_hashHandler(a_hashHandler)
, m_totalKeyValuePairs(0) {
    if(!a_initialSize) {
        throw std::invalid_argument("Wrong Size Error");
    }
}


template <typename K, typename V>
void HashMap::HashMap<K,V>::Insert(const K& a_key, const V& a_value) {

    // TODO

    ++this->m_totalKeyValuePairs;
}


template <typename K, typename V>
void HashMap::HashMap<K,V>::Remove(const K& a_key) {

    // TODO

    --this->m_totalKeyValuePairs;
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::Find(const K& a_key) const {

    // TODO
}


template <typename K, typename V>
size_t HashMap::HashMap<K,V>::Size() const {
    return this->m_totalKeyValuePairs;
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::Begin() const {
    return HashMapIterator<K,V>(this->m_buckets.begin(), this->m_buckets.end());
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::End() const {
    return HashMapIterator<K,V>(this->m_buckets.end(), this->m_buckets.end());
}


template <typename K, typename V>
typename std::list<HashMap::KeyValuePair<K,V>>::iterator HashMap::HashMap<K,V>::SearchByKey(const K& a_key) const {
    // TODO
}


#endif // __HASHMAP_HASHMAP_INLINE_HPP__