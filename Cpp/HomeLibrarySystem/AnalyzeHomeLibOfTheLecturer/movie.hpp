#ifndef __MOVIE_ITEM__
#define __MOVIE_ITEM__


#include <string> // std::string
#include "library_item.hpp"


namespace homelib {


class Movie : public LibraryItem {
public:
    enum Genere { ACTION, HORROR, COMDEY, FANTASY, ANIME, OTHER };
    Movie(const std::string &a_title, Genere a_genere, unsigned short a_yearOfPublication);
    Movie(const Movie &a_other) = default;
    Movie &operator=(const Movie &a_other) = default;
    ~Movie() = default;

    Genere GetGenere() const;
    std::string GetGenereName() const;
    void Print(std::ostream &a_out) const;

private:
    Genere m_genere;
};


} // homelib


#endif // __MOVIE_ITEM__