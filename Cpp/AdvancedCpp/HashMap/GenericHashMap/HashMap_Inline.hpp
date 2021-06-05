#ifndef __HASHMAP_HASHMAP_INLINE_HPP__
#define __HASHMAP_HASHMAP_INLINE_HPP__


#include "HashMap.hpp"
#include <cstddef> // size_t
#include <stdexcept> // std::invalid_argument
#include "HashMapTypes.hpp"
#include "KeyValuePair.hpp"
#include "KeyValuePair_Inline.hpp"
#include "HashMapIterator.hpp"
#include "HashMapIterator_Inline.hpp"
#include "KeyHashingHandler.hpp"


template <typename K, typename V>
HashMap::HashMap<K,V>::HashMap(const size_t a_initialSize, const KeyHashingHandler<K>& a_hashHandler)
: m_buckets(a_initialSize) // Every list is initialized using default c'tor (num of {a_initialSize} lists)
, m_hashHandler(a_hashHandler)
, m_totalKeyValuePairs(0) {
    if(a_initialSize == 0) {
        throw std::invalid_argument("Wrong Size Error");
    }
}


template <typename K, typename V>
bool HashMap::HashMap<K,V>::Insert(const K& a_key, const V& a_value) {
    size_t bucketNumber = this->m_hashHandler.Hash(a_key) % this->m_buckets.size();

    typename Types<K,V>::InnerContainer::iterator bucketIterator = this->SearchByKey(bucketNumber, a_key); // Try to find if the key is duplicated (key exists already)
    if(bucketIterator != this->m_buckets.at(bucketNumber).end()) { // Found the same key
        return false;
    }

    // Key not found - insert the pair:
    this->m_buckets.at(bucketNumber).push_back(typename Types<K,V>::KeyVal(a_key, a_value));
    ++this->m_totalKeyValuePairs;

    return true;
}


template <typename K, typename V>
bool HashMap::HashMap<K,V>::Remove(const K& a_key) {
    size_t bucketNumber = this->m_hashHandler.Hash(a_key) % this->m_buckets.size();

    typename Types<K,V>::InnerContainer::iterator bucketIterator = this->SearchByKey(bucketNumber, a_key); // Try to find the key (checks if key exists)
    if(bucketIterator == this->m_buckets.at(bucketNumber).end()) { // Failed to find the key
        return false;
    }

    // Key found - remove the pair:
    this->m_buckets.at(bucketNumber).erase(bucketIterator);
    --this->m_totalKeyValuePairs;

    return true;
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::Find(const K& a_key) const {
    size_t bucketNumber = this->m_hashHandler.Hash(a_key) % this->m_buckets.size();

    typename Types<K,V>::InnerContainer::iterator bucketIterator = this->SearchByKey(bucketNumber, a_key); // Try to find the key (checks if key exists)
    if(bucketIterator == this->m_buckets.at(bucketNumber).end()) { // Failed to find the key
        return this->End();
    }

    // Key found:
    typename Types<K,V>::OuterContainer::iterator currentBucketsIterator = const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).begin() + bucketNumber;

    return HashMapIterator<K,V>(currentBucketsIterator, const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).end(), bucketIterator, bucketNumber);
}


template <typename K, typename V>
size_t HashMap::HashMap<K,V>::Size() const {
    return this->m_totalKeyValuePairs;
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::Begin() const {
    return HashMapIterator<K,V>(const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).begin(), const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).end());
}


template <typename K, typename V>
typename HashMap::HashMap<K,V>::Iterator HashMap::HashMap<K,V>::End() const {
    return HashMapIterator<K,V>(const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).end(), const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).end());
}


template <typename K, typename V>
typename HashMap::Types<K,V>::InnerContainer::iterator HashMap::HashMap<K,V>::SearchByKey(const size_t a_bucketNumber, const K& a_key) const {
    typename Types<K,V>::InnerContainer::iterator currentBucketIterator = const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).at(a_bucketNumber).begin();
    typename Types<K,V>::InnerContainer::iterator endBucketIterator = const_cast<typename Types<K,V>::OuterContainer&>(this->m_buckets).at(a_bucketNumber).end();

    while(currentBucketIterator != endBucketIterator) {
        if((*currentBucketIterator).GetKey() == a_key) {
            break; // Found the key - the current iterator "points" to it
        }

        ++currentBucketIterator;
    }

    return currentBucketIterator; // end would be returned if key not found
}


#endif // __HASHMAP_HASHMAP_INLINE_HPP__