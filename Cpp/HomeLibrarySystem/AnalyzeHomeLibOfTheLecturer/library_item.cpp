#include "library_item.hpp"
#include <iostream> // std::ostream


homelib::LibraryItem::LibraryItem(const std::string &a_title, unsigned
short a_yearOfPublication)
: m_title(a_title)
, m_yearOfPublication(a_yearOfPublication) {
}


const std::string &homelib::LibraryItem::GetTitle() const {
    return m_title;
}


unsigned short homelib::LibraryItem::GetYearOfPublication() const {
    return m_yearOfPublication;
}


void homelib::LibraryItem::Print(std::ostream &a_out) const {
    a_out << GetTitle() << ", " << GetYearOfPublication();
}