// A LibraryIterator class, to manage the iteration of the collection of a Library<T>
#ifndef __LIBRARYITERATOR_HPP__
#define __LIBRARYITERATOR_HPP__


#include <list> // std::list


template <typename T>
class LibraryIterator {
public:
    LibraryIterator(const typename std::list<T>::const_iterator& a_iterator);
    LibraryIterator(const LibraryIterator<T>& a_other);
    LibraryIterator<T>& operator=(const LibraryIterator<T>& a_other);
    ~LibraryIterator();


    // Operator Overloading:
    LibraryIterator<T>& operator++();
    LibraryIterator<T>& operator--();
    const T& operator*();
    bool operator==(const LibraryIterator<T>& a_other) const;
    bool operator!=(const LibraryIterator<T>& a_other) const;

private:
    typename std::list<T>::const_iterator m_iterator;
};


#endif // __LIBRARYITERATOR_HPP__