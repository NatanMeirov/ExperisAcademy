#include "movie.hpp"
#include <iostream> // std::ostream


static const char *g_genereLut[] = {
    "action", "horror", "comdey", "fantasy", "anime", "other"
};


homelib::Movie::Movie(const std::string &a_title, Genere a_genere, unsigned short a_yearOfPublication)
: LibraryItem(a_title, a_yearOfPublication)
, m_genere(a_genere) {
}


std::string homelib::Movie::GetGenereName() const { // QUESTION: Why can't I return by const-reference? ("const std::string &") //! Natan: Because the string's array from above is a const char* string's array, and every time this function is called - the compiler is calling a c'tor of std::string that gets const char*, AND HERE - ITS CREATED ON THE STACK! (the std::string) - so it is a wrong thing to return a reference of a stack allocated var
    return g_genereLut[m_genere]; // QUESTION: Is this a std::string? Why does it compile? What rewrite does the compiler perform? //! Natan: no, its a c-string (const char*), initialized on the data section, and the compiler is creates of it (using a const char* c'tor) a std::string instance ON THE STACK, and returns a COPY of it (returns by value, using the copy c'tor)
}


void homelib::Movie::Print(std::ostream &a_out) const { // QUESTION: Why can I call GetTitle() from this method? //! Natan: because the Movie object inherits (using PUBLIC access modifier) from the LibraryItem class - so all the PUBLIC LibraryItem class' members and methods are public to self (Movie instance), and can be used in the Movie's code
    LibraryItem::Print(a_out); // QUESTION: what does this do? //! Natan: makes a call to the LibraryItem's part of self (a Movie instance) - to print the "inner" LibraryItem's members of self (Movie)
    a_out << " (" << GetGenereName() << ")";
}