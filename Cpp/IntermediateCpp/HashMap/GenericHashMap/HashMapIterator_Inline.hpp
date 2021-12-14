#ifndef __HASHMAP_HASHMAPITERATOR_INLINE_HPP__
#define __HASHMAP_HASHMAPITERATOR_INLINE_HPP__


#include "HashMapIterator.hpp"
#include "HashMapTypes.hpp"
#include "KeyValuePair.hpp"
#include "KeyValuePair_Inline.hpp"


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(OuterContainerIterator a_outerContainerBeginIterator, OuterContainerIterator a_outerContainerEndIterator)
: m_outerContainerCurrentIterator(a_outerContainerBeginIterator)
, m_outerContainerEndIterator(a_outerContainerEndIterator)
, m_innerContainerCurrentIterator()
, m_innerContainerEndIterator()
, m_offsetFromCurrentToBeginOfOuterContainerIterator(0) {
    // The inner iterators cannot be on the MIL on the standard Begin/End cases (_outerContainerBeginIterator can be end() - and it shall not dereferenced)
    if(a_outerContainerBeginIterator != a_outerContainerEndIterator) { // ONLY if it is NOT the End of HashMap (HashMap End() Iterator)
        this->m_innerContainerCurrentIterator = (*a_outerContainerBeginIterator).begin();
        this->m_innerContainerEndIterator = (*a_outerContainerBeginIterator).end();
        this->EnsureIteratorLocationIsValidOrFindNextValid();
    }
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(OuterContainerIterator a_outerContainerCurrentIterator, OuterContainerIterator a_outerContainerEndIterator, InnerContainerIterator a_innerContainerCurrentIterator, const size_t a_offsetFromCurrentToBeginOfOuterContainerIterator)
: m_outerContainerCurrentIterator(a_outerContainerCurrentIterator)
, m_outerContainerEndIterator(a_outerContainerEndIterator)
, m_innerContainerCurrentIterator(a_innerContainerCurrentIterator)
, m_innerContainerEndIterator((*a_outerContainerCurrentIterator).end())
, m_offsetFromCurrentToBeginOfOuterContainerIterator(a_offsetFromCurrentToBeginOfOuterContainerIterator) {
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(const HashMapIterator<K,V>& a_other)
: m_outerContainerCurrentIterator(a_other.m_outerContainerCurrentIterator)
, m_outerContainerEndIterator(a_other.m_outerContainerEndIterator)
, m_innerContainerCurrentIterator(a_other.m_innerContainerCurrentIterator)
, m_innerContainerEndIterator(a_other.m_innerContainerEndIterator)
, m_offsetFromCurrentToBeginOfOuterContainerIterator(a_other.m_offsetFromCurrentToBeginOfOuterContainerIterator) {
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator=(const HashMapIterator<K,V>& a_other) {
    this->m_outerContainerCurrentIterator = a_other.m_outerContainerCurrentIterator;
    this->m_outerContainerEndIterator = a_other.m_outerContainerEndIterator;
    this->m_innerContainerCurrentIterator = a_other.m_innerContainerCurrentIterator;
    this->m_innerContainerEndIterator = a_other.m_innerContainerEndIterator;
    this->m_offsetFromCurrentToBeginOfOuterContainerIterator = a_other.m_offsetFromCurrentToBeginOfOuterContainerIterator;

    return *this;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator++() {
    this->PlaceIteratorOnNextValidLocation();

    return *this;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator--() {
    this->PlaceIteratorOnPreviousValidLocation();

    return *this;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator+=(const size_t a_offset) {
    return *this = *this + a_offset;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator-=(const size_t a_offset) {
    return *this = *this - a_offset;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V> HashMap::HashMapIterator<K,V>::operator+(const size_t a_offset) {
    HashMapIterator<K,V> iterator(*this);
    for(size_t i = 0; i < a_offset && !iterator.IsEnd(); ++i) {
        iterator.operator++();
    }

    return iterator;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V> HashMap::HashMapIterator<K,V>::operator-(const size_t a_offset) {
    HashMapIterator<K,V> iterator(*this);
    for(size_t i = 0; i < a_offset && !iterator.IsBegin(); ++i) {
        iterator.operator--();
    }

    return iterator;
}



template <typename K, typename V>
V& HashMap::HashMapIterator<K,V>::operator*() const {
    return (*(this->m_innerContainerCurrentIterator)).GetValue();
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::operator!=(const HashMapIterator<K,V>& a_other) const {
    if(this->m_outerContainerCurrentIterator != a_other.m_outerContainerCurrentIterator) {
        return true;
    }
    // Else - Both m_outerContainerCurrentIterator are equal

    if(this->IsEnd()) { // both are equal to outerContainerEndIterator (both "pointing to the end - innerContainer's iterators are not a factor in that case")
        return false;
    }
    // Else - They are on the same outer container's place ("pointing" to the same inner container) - check their equality by looking at their outerContainterCurrentIterators (if they are equal)

    return this->m_innerContainerCurrentIterator != a_other.m_innerContainerCurrentIterator;
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::operator==(const HashMapIterator<K,V>& a_other) const {
    return !(*this != a_other); // Check by using Not on the opposite
}


template <typename K, typename V>
void HashMap::HashMapIterator<K,V>::PlaceIteratorOnNextValidLocation() {
    ++this->m_innerContainerCurrentIterator;
    this->EnsureIteratorLocationIsValidOrFindNextValid();
}


template <typename K, typename V>
void HashMap::HashMapIterator<K,V>::EnsureIteratorLocationIsValidOrFindNextValid() {
    while(!this->IsEnd() && this->m_innerContainerCurrentIterator == this->m_innerContainerEndIterator) {
        ++this->m_outerContainerCurrentIterator;
        ++this->m_offsetFromCurrentToBeginOfOuterContainerIterator;
        if(!this->IsEnd()) { // If Now (after the ++) - still did not reach the end of the whole outer container
            this->m_innerContainerCurrentIterator = (*(this->m_outerContainerCurrentIterator)).begin();
            this->m_innerContainerEndIterator = (*(this->m_outerContainerCurrentIterator)).end();
        }
    }
}


template <typename K, typename V>
void HashMap::HashMapIterator<K,V>::PlaceIteratorOnPreviousValidLocation() {

}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::IsBegin() const {
    return (this->m_outerContainerCurrentIterator - this->m_offsetFromCurrentToBeginOfOuterContainerIterator).begin() == this->m_innerContainerCurrentIterator;
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::IsEnd() const {
    return this->m_outerContainerCurrentIterator == this->m_outerContainerEndIterator;
}


#endif // __HASHMAP_HASHMAPITERATOR_INLINE_HPP__