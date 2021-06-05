#ifndef __BOOK_HPP__
#define __BOOK_HPP__


#include <string> // std::string
#include "ArtWork.hpp"


class Book : public ArtWork {
public:

    Book(const std::string& a_bookName, const std::string& a_authorName, const unsigned int& a_publishYear);
    Book(const Book& a_other);
    Book& operator=(const Book& a_other);
    ~Book();
};


#endif // __BOOK_HPP__