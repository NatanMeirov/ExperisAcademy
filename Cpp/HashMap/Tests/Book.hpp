#ifndef __BOOK_HPP__
#define __BOOK_HPP__


#include <string> //std::string


class Book {
public:
    Book(const std::string& a_bookName, const std::string& a_authorName, const unsigned int a_publishYear, const unsigned int a_receivedYear);
    Book(const Book& a_otherBook) = default;
    Book& operator=(const Book& a_otherBook) = default;
    ~Book() = default;


    std::string GetBookName() const;
    std::string GetAuthorName() const;
    const unsigned int GetPublishYear() const;
    const unsigned int GetReceivedYear() const;

private:
    std::string m_bookName;
    std::string m_authorName;
    unsigned int m_publishYear;
    unsigned int m_receivedYear;
};


#endif // __BOOK_HPP__