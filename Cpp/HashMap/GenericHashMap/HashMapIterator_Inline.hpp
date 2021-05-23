#ifndef __HASHMAP_HASHMAPITERATOR_INLINE_HPP__
#define __HASHMAP_HASHMAPITERATOR_INLINE_HPP__


#include "HashMapIterator.hpp"
#include <vector>
#include <list>
#include "KeyValuePair_Inline.hpp"


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorBeginIterator, typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorEndIterator)
: m_vectorCurrentIterator(a_vectorBeginIterator)
, m_vectorEndIterator(a_vectorEndIterator)
, m_listCurrentIterator()
, m_listEndIterator() {
    // The list iterators cannot be on the MIL
    if(a_vectorBeginIterator != a_vectorEndIterator) { // ONLY if it is NOT the End HashMap Iterator
        this->m_listCurrentIterator = (*a_vectorBeginIterator).begin();
        this->m_listEndIterator = (*a_vectorBeginIterator).end();

        this->PlaceIteratorOnNextValidLocation(true);
    }
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorCurrentIterator, typename std::vector<std::list<KeyValuePair<K,V>>>::iterator a_vectorEndIterator, typename std::list<KeyValuePair<K,V>>::iterator a_listCurrentIterator)
: m_vectorCurrentIterator(a_vectorCurrentIterator)
, m_vectorEndIterator(a_vectorEndIterator)
, m_listCurrentIterator(a_listCurrentIterator)
, m_listEndIterator((*a_vectorCurrentIterator).end()){
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator++() {
    this->PlaceIteratorOnNextValidLocation();

    return *this;
}


template <typename K, typename V>
V& HashMap::HashMapIterator<K,V>::operator*() {
    return (*(this->m_listCurrentIterator)).GetValueByRef();
}


template <typename K, typename V>
V HashMap::HashMapIterator<K,V>::operator*() const {
    return (*(this->m_listCurrentIterator)).GetValue();
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::operator!=(const HashMapIterator<K,V>& a_other) {
    if(this->m_vectorCurrentIterator != a_other.m_vectorCurrentIterator) {
        return true;
    }

    // Else - Both vectorCurrentIterators are equal
    if(this->m_vectorCurrentIterator == this->m_vectorEndIterator) { // == a_other.m_vectorCurrentIterator - both are equal to vectorEndIterator
        return false;
    }

    // Else - They are on the same vector's place (same list) - check their equality by looking at their listCurrentIterators
    return this->m_listCurrentIterator != a_other.m_listCurrentIterator;
}


template <typename K, typename V>
void HashMap::HashMapIterator<K,V>::PlaceIteratorOnNextValidLocation(bool a_isInitializationPart) {
    bool hasPlacedCorrectly = false;

    if(a_isInitializationPart) {
        if(this->m_listCurrentIterator != this->m_listEndIterator) {
            return; // No change is required
        }
    }

    while(this->m_vectorCurrentIterator != this->m_vectorEndIterator) {
        if(this->m_listCurrentIterator != this->m_listEndIterator) {
            ++this->m_listCurrentIterator;
        }

        if(this->m_listCurrentIterator == this->m_listEndIterator) { // If Now (after the ++) - reached the end of the currently iterated list
            ++this->m_vectorCurrentIterator;
            if(this->m_vectorCurrentIterator != this->m_vectorEndIterator) { // If Now (after the ++) - still did not reach the end of the whole vector
                this->m_listCurrentIterator = (*(this->m_vectorCurrentIterator)).begin();
                this->m_listEndIterator = (*(this->m_vectorCurrentIterator)).end();
            }
        }
        else { // this->m_listCurrentIterator != this->m_listEndIterator
            break; // Then - Iterator has placed correctly
        }
    }
}


#endif // __HASHMAP_HASHMAPITERATOR_INLINE_HPP__