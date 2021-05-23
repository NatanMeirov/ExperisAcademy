#ifndef __MOVIE_HPP__
#define __MOVIE_HPP__


#include <string> // std::string
#include "ArtWork.hpp"


class Movie : public ArtWork {
public:

    enum Genre {
        ACTION,
        COMEDY,
        DRAMA,
        ANIME,
        OTHER_GENRE
    };


    Movie(const std::string& a_movieName, const std::string& a_directorName, const Genre& a_genre, const unsigned int& a_publishYear);
    Movie(const Movie& a_other);
    Movie& operator=(const Movie& a_other);
    ~Movie();


    const Genre& GetGenre() const;
    const char* GetGenreAsString() const;


private:

    Genre m_genre;
};


#endif // __MOVIE_HPP__