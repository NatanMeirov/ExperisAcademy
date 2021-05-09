#include "Movie.hpp"
#include <cstddef> // size_t
#include <cstring> // strlen, strcpy
#include <stdexcept> // std::invalid_argument
#include "../UtilFunctions.hpp"


Movie::Movie(const char* a_movieName, const char* a_director, Genre a_genre, unsigned int a_releasedYear, unsigned int a_receivedYear)
: m_movieName(new char[SafeStrlenWrapper(a_movieName) + 1])
, m_director(new char[SafeStrlenWrapper(a_director) + 1])
, m_genre(a_genre)
, m_releasedYear(a_releasedYear)
, m_receivedYear(a_receivedYear) {
    if(a_genre < Movie::ACTION || a_genre > Movie::OTHER_GENRE || this->m_releasedYear < Movie::MOVIE_MIN_RELEASED_YEAR || this->m_releasedYear > this->m_receivedYear) {
        throw std::invalid_argument("Invalid Argument Error");
    }

    strcpy(this->m_movieName, a_movieName);
    strcpy(this->m_director, a_director);
}


Movie::Movie(const Movie& a_otherMovie)
: m_movieName(new char[strlen(a_otherMovie.m_movieName) + 1])
, m_director(new char[strlen(a_otherMovie.m_director) + 1])
, m_genre(a_otherMovie.m_genre)
, m_releasedYear(a_otherMovie.m_releasedYear)
, m_receivedYear(a_otherMovie.m_receivedYear) {
    strcpy(this->m_movieName, a_otherMovie.m_movieName);
    strcpy(this->m_director, a_otherMovie.m_director);
}


Movie& Movie::operator=(const Movie &a_otherMovie) {
    delete[] this->m_movieName;
    delete[] this->m_director;

    this->m_movieName = new char [strlen(a_otherMovie.m_movieName) + 1];
    this->m_director = new char [strlen(a_otherMovie.m_director) + 1];

    strcpy(this->m_movieName, a_otherMovie.m_movieName);
    strcpy(this->m_director, a_otherMovie.m_director);

    this->m_genre = a_otherMovie.m_genre;
    this->m_releasedYear = a_otherMovie.m_releasedYear;
    this->m_receivedYear = a_otherMovie.m_receivedYear;

    return *this;
}


Movie::~Movie() {
    delete[] this->m_movieName;
    delete[] this->m_director;
}


const char* Movie::GetMovieName() const {
    return this->m_movieName;
}


const char* Movie::GetDirector() const {
    return this->m_director;
}


const Movie::Genre Movie::GetGenre() const {
    return this->m_genre;
}


const unsigned int Movie::GetReleasedYear() const {
    return this->m_releasedYear;
}


const unsigned int Movie::GetReceivedYear() const {
    return this->m_receivedYear;
}


const char* Movie::GetGenreAsString() const {
    switch (this->m_genre) {
    case Movie::ACTION:
        return "Action";

    case Movie::COMEDY:
        return "Comedy";

    case Movie::DRAMA:
        return "Drama";

    case Movie::ANIME:
        return "Anime";

    case Movie::OTHER_GENRE:
        return "Other";
    }

    return "Unknow Genre";
}