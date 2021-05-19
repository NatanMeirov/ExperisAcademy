#include "book.hpp"
#include <iostream> // std::ostream


homelib::Book::Book(const std::string &a_title, const std::string &a_auther, unsigned short a_yearOfPublication)
: LibraryItem(a_title, a_yearOfPublication)
, m_auther(a_auther) {
}


const std::string &homelib::Book::GetAuther() const {
    return m_auther;
}


void homelib::Book::Print(std::ostream &a_out) const {
    a_out << GetTitle() << " / " << GetAuther() << ", " << GetYearOfPublication();
}