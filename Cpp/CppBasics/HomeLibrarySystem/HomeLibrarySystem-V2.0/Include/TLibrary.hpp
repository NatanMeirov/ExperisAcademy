#ifndef __TLIBRARY_HPP__
#define __TLIBRARY_HPP__


#include <list> // std::list
#include "TLibraryIterator.hpp"


template <typename T>
class Library {
public:
    typedef LibraryIterator<T> Iterator;

    Library();
    Library(const Library<T>& a_other);
    Library<T>& operator=(const Library<T>& a_other);
    ~Library();


    void Add(T a_toAdd);
    Iterator Begin() const;
    Iterator End() const;

private:
    std::list<T> m_libraryCollection;
};


#endif // __TLIBRARY_HPP__