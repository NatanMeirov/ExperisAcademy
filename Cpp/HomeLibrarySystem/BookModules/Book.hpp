#ifndef __BOOK_HPP__
#define __BOOK_HPP__


class Book {
public:

    static const int BOOK_MIN_RELEASED_YEAR = 1900;

    Book(const char* a_bookName, const char* a_author, const unsigned int a_releasedYear, const unsigned int a_receivedYear);
    Book(const Book& a_otherBook);
    Book& operator=(const Book& a_otherBook);
    ~Book();


    const char* GetBookName() const;
    const char* GetAuthor() const;
    const unsigned int GetReleasedYear() const;
    const unsigned int GetReceivedYear() const;


private:

    char* m_bookName;
    char* m_author;
    unsigned int m_releasedYear;
    unsigned int m_receivedYear;
};


#endif // __BOOK_HPP__