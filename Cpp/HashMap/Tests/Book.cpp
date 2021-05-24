#include "../Include/Book.hpp"


Book::Book(const std::string& a_bookName, const std::string& a_authorName, const unsigned int a_publishYear, const unsigned int a_receivedYear)
: m_bookName(a_bookName)
, m_authorName(a_authorName)
, m_publishYear(a_publishYear)
, m_receivedYear(a_receivedYear) {
}


std::string Book::GetBookName() const {
    return this->m_bookName;
}


std::string Book::GetAuthorName() const {
    return this->m_authorName;
}


const unsigned int Book::GetPublishYear() const {
    return this->m_publishYear;
}


const unsigned int Book::GetReceivedYear() const {
    return this->m_receivedYear;
}