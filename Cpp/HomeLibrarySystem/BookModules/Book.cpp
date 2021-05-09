#include "Book.hpp"
#include <cstddef> // size_t
#include <cstring> // strlen, strcpy
#include <stdexcept> // std::invalid_argument
#include "../UtilFunctions.hpp"


Book::Book(const char* a_bookName, const char* a_author, const unsigned int a_releasedYear, const unsigned int a_receivedYear)
: m_bookName(new char[SafeStrlenWrapper(a_bookName) + 1])
, m_author(new char[SafeStrlenWrapper(a_author) + 1])
, m_releasedYear(a_releasedYear)
, m_receivedYear(a_receivedYear) {
    if(this->m_releasedYear < Book::BOOK_MIN_RELEASED_YEAR || this->m_releasedYear > this->m_receivedYear) {
        throw std::invalid_argument("Invalid Argument Error");
    }

    strcpy(this->m_bookName, a_bookName);
    strcpy(this->m_author, a_author);
}


Book::Book(const Book& a_otherBook)
: m_bookName(new char[strlen(a_otherBook.m_bookName) + 1])
, m_author(new char[strlen(a_otherBook.m_author) + 1])
, m_releasedYear(a_otherBook.m_releasedYear)
, m_receivedYear(a_otherBook.m_receivedYear) {
    strcpy(this->m_bookName, a_otherBook.m_bookName);
    strcpy(this->m_author, a_otherBook.m_author);
}


Book& Book::operator=(const Book &a_otherBook) {
    delete[] this->m_bookName;
    delete[] this->m_author;

    this->m_bookName = new char [strlen(a_otherBook.m_bookName) + 1];
    this->m_author = new char [strlen(a_otherBook.m_author) + 1];

    strcpy(this->m_bookName, a_otherBook.m_bookName);
    strcpy(this->m_author, a_otherBook.m_author);

    this->m_releasedYear = a_otherBook.m_releasedYear;
    this->m_receivedYear = a_otherBook.m_receivedYear;

    return *this;
}


Book::~Book() {
    delete[] this->m_bookName;
    delete[] this->m_author;
}


const char* Book::GetBookName() const {
    return this->m_bookName;
}


const char* Book::GetAuthor() const {
    return this->m_author;
}


const unsigned int Book::GetReleasedYear() const {
    return this->m_releasedYear;
}


const unsigned int Book::GetReceivedYear() const {
    return this->m_receivedYear;
}