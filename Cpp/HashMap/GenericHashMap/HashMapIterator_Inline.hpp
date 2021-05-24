#ifndef __HASHMAP_HASHMAPITERATOR_INLINE_HPP__
#define __HASHMAP_HASHMAPITERATOR_INLINE_HPP__


#include "HashMapIterator.hpp"
#include "HashMapTypes.hpp"
#include "KeyValuePair.hpp"
#include "KeyValuePair_Inline.hpp"


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(typename Types<K,V>::OuterContainer::iterator a_outerContainerBeginIterator, typename Types<K,V>::OuterContainer::iterator a_outerContainerEndIterator)
: m_outerContainerCurrentIterator(a_outerContainerBeginIterator)
, m_outerContainerEndIterator(a_outerContainerEndIterator)
, m_innerContainerCurrentIterator()
, m_innerContainerEndIterator() {
    // The inner iterators cannot be on the MIL on the standard Begin/End cases (_outerContainerBeginIterator can be end() - and it shall not dereferenced)
    if(a_outerContainerBeginIterator != a_outerContainerEndIterator) { // ONLY if it is NOT the End of HashMap (HashMap End() Iterator)
        this->m_innerContainerCurrentIterator = (*a_outerContainerBeginIterator).begin();
        this->m_innerContainerEndIterator = (*a_outerContainerBeginIterator).end();

        if(this->m_innerContainerCurrentIterator == this->m_innerContainerEndIterator) {
            this->PlaceIteratorOnNextValidLocation();
        }
    }
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(typename Types<K,V>::OuterContainer::iterator a_outerContainerCurrentIterator, typename Types<K,V>::OuterContainer::iterator a_outerContainerEndIterator, typename Types<K,V>::InnerContainer::iterator a_innerContainerCurrentIterator)
: m_outerContainerCurrentIterator(a_outerContainerCurrentIterator)
, m_outerContainerEndIterator(a_outerContainerEndIterator)
, m_innerContainerCurrentIterator(a_innerContainerCurrentIterator)
, m_innerContainerEndIterator((*a_outerContainerCurrentIterator).end()){
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>::HashMapIterator(const HashMapIterator<K,V>& a_other)
: m_outerContainerCurrentIterator(a_other.m_outerContainerCurrentIterator)
, m_outerContainerEndIterator(a_other.m_outerContainerEndIterator)
, m_innerContainerCurrentIterator(a_other.m_innerContainerCurrentIterator)
, m_innerContainerEndIterator(a_other.m_innerContainerEndIterator){
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator=(const HashMapIterator<K,V>& a_other) {
    this->m_outerContainerCurrentIterator = a_other.m_outerContainerCurrentIterator;
    this->m_outerContainerEndIterator = a_other.m_outerContainerEndIterator;
    this->m_innerContainerCurrentIterator = a_other.m_innerContainerCurrentIterator;
    this->m_innerContainerEndIterator = a_other.m_innerContainerEndIterator;

    return *this;
}


template <typename K, typename V>
HashMap::HashMapIterator<K,V>& HashMap::HashMapIterator<K,V>::operator++() {
    this->PlaceIteratorOnNextValidLocation();

    return *this;
}


template <typename K, typename V>
V& HashMap::HashMapIterator<K,V>::operator*() {
    return (*(this->m_innerContainerCurrentIterator)).GetValueByRef();
}


template <typename K, typename V>
V HashMap::HashMapIterator<K,V>::operator*() const {
    return (*(this->m_innerContainerCurrentIterator)).GetValue();
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::operator!=(const HashMapIterator<K,V>& a_other) {
    if(this->m_outerContainerCurrentIterator != a_other.m_outerContainerCurrentIterator) {
        return true;
    }

    // Else - Both vectorCurrentIterators are equal
    if(this->m_outerContainerCurrentIterator == this->m_outerContainerEndIterator) { // == a_other.m_outerContainerCurrentIterator - both are equal to vectorEndIterator
        return false;
    }

    // Else - They are on the same vector's place (same list) - check their equality by looking at their listCurrentIterators
    return this->m_innerContainerCurrentIterator != a_other.m_innerContainerCurrentIterator;
}


template <typename K, typename V>
bool HashMap::HashMapIterator<K,V>::operator==(const HashMapIterator<K,V>& a_other) {
    return this->m_innerContainerCurrentIterator == a_other.m_innerContainerCurrentIterator;
}


template <typename K, typename V>
void HashMap::HashMapIterator<K,V>::PlaceIteratorOnNextValidLocation() {
    bool hasAtleastOneIncrementingOccurred = false;
    while(this->m_outerContainerCurrentIterator != this->m_outerContainerEndIterator) {
        if(this->m_innerContainerCurrentIterator != this->m_innerContainerEndIterator) {
            if(hasAtleastOneIncrementingOccurred) {
                break; // Found - can stop now (placed the iterator on a valid location
            }

            ++this->m_innerContainerCurrentIterator;
        }

        if(this->m_innerContainerCurrentIterator == this->m_innerContainerEndIterator) { // If Now (after the ++) - reached the end of the currently iterated inner container
            ++this->m_outerContainerCurrentIterator;
            if(this->m_outerContainerCurrentIterator != this->m_outerContainerEndIterator) { // If Now (after the ++) - still did not reach the end of the whole outer container
                this->m_innerContainerCurrentIterator = (*(this->m_outerContainerCurrentIterator)).begin();
                this->m_innerContainerEndIterator = (*(this->m_outerContainerCurrentIterator)).end();
            }
        }

        if(!hasAtleastOneIncrementingOccurred) {
            hasAtleastOneIncrementingOccurred = true;
        }
    }
}


#endif // __HASHMAP_HASHMAPITERATOR_INLINE_HPP__