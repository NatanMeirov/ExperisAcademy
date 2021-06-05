#ifndef __LIBRARY_ITEM__
#define __LIBRARY_ITEM__


#include <string> // std::string
#include <iosfwd> // ostream


namespace homelib { // QUESTION: What does this do? // !Natan: a new scope resolution, named: homelib


class LibraryItem {
public:
    LibraryItem(const std::string &a_title, unsigned short
    a_yearOfPublication);
    LibraryItem(const LibraryItem &a_other) = default;
    LibraryItem &operator=(const LibraryItem &a_other) = default;
    ~LibraryItem() = default;

    const std::string &GetTitle() const;
    unsigned short GetYearOfPublication() const;
    void Print(std::ostream &a_out) const;

private:
    std::string m_title;
    unsigned short m_yearOfPublication;
};


} // homelib


#endif // __LIBRARY_ITEM__