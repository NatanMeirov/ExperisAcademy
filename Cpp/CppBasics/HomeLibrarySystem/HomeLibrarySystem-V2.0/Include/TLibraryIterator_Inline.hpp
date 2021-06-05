#include "TLibraryIterator.hpp"
#include <list> // std::list


template <typename T>
LibraryIterator<T>::LibraryIterator(const typename std::list<T>::const_iterator& a_iterator)
: m_iterator(a_iterator) {
}


template <typename T>
LibraryIterator<T>::LibraryIterator(const LibraryIterator& a_other)
: m_iterator(a_other.m_iterator) {
}


template <typename T>
LibraryIterator<T>& LibraryIterator<T>::operator=(const LibraryIterator& a_other) {
    this->m_iterator = a_other.m_iterator;
}


template <typename T>
LibraryIterator<T>::~LibraryIterator<T>() {
}


template <typename T>
LibraryIterator<T>& LibraryIterator<T>::operator++() {
    this->m_iterator++;

    return *this;
}


template <typename T>
LibraryIterator<T>& LibraryIterator<T>::operator--() {
    this->m_iterator--;

    return *this;
}


template <typename T>
const T& LibraryIterator<T>::operator*() {
    return *(this->m_iterator);
}


template <typename T>
bool LibraryIterator<T>::operator==(const LibraryIterator<T>& a_other) const {
    return this->m_iterator == a_other.m_iterator;
}


template <typename T>
bool LibraryIterator<T>::operator!=(const LibraryIterator<T>& a_other) const {
    return this->m_iterator != a_other.m_iterator;
}