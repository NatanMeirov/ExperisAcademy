#ifndef __BOOK_ITEM__
#define __BOOK_ITEM__


#include <string> // std::string
#include "library_item.hpp"


namespace homelib {


class Book : public LibraryItem {
public:
    Book(const std::string &a_title, const std::string &a_auther, unsigned short a_yearOfPublication);
    Book(const Book &a_other) = default;
    Book &operator=(const Book &a_other) = default;
    ~Book() = default;

    const std::string &GetAuther() const;
    void Print(std::ostream &a_out) const;

private:
    std::string m_auther;
};


} // homelib


#endif // __BOOK_ITEM__