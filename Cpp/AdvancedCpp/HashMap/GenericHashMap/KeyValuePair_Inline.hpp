#ifndef __HASHMAP_KEYVALUEPAIR_INLINE_HPP__
#define __HASHMAP_KEYVALUEPAIR_INLINE_HPP__


#include "KeyValuePair.hpp"


template <typename K, typename V>
HashMap::KeyValuePair<K,V>::KeyValuePair(const K& a_key, const V& a_value)
: m_key(a_key)
, m_value(a_value) {
}


template <typename K, typename V>
K HashMap::KeyValuePair<K,V>::GetKey() const {
    return this->m_key;
}


template <typename K, typename V>
V& HashMap::KeyValuePair<K,V>::GetValue() {
    return this->m_value;
}


#endif // __HASHMAP_KEYVALUEPAIR_INLINE_HPP__