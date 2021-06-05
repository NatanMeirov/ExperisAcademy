// Inline definition of TLibrary


#ifndef __TLIBRARY_INLINE_HPP__
#define __TLIBRARY_INLINE_HPP__


#include "TLibrary.hpp"
#include "TLibraryIterator.hpp"
#include <list> // std::list


template <typename T>
Library<T>::Library()
: m_libraryCollection() {
}

template <typename T>
Library<T>::Library(const Library<T>& a_other)
: m_libraryCollection(a_other.m_libraryCollection) {
}


template <typename T>
Library<T>& Library<T>::operator=(const Library<T>& a_other){
    this->m_libraryCollection = a_other.m_libraryCollection;

    return *this;
}


template <typename T>
Library<T>::~Library() {
}


template <typename T>
void Library<T>::Add(T a_toAdd) {
    this->m_libraryCollection.push_back(a_toAdd);
}


template <typename T>
typename Library<T>::Iterator Library<T>::Begin() const {
    return LibraryIterator<T>(this->m_libraryCollection.begin());
}


template <typename T>
typename Library<T>::Iterator Library<T>::End() const {
    return LibraryIterator<T>(this->m_libraryCollection.end());
}


#endif // __TLIBRARY_INLINE_HPP__
