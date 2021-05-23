#include "../Include/Movie.hpp"
#include <string> // std::string
#include "../Include/ArtWork.hpp"


Movie::Movie(const std::string& a_movieName, const std::string& a_directorName, const Genre& a_genre, const unsigned int& a_publishYear)
: ArtWork(a_movieName, a_directorName, a_publishYear, std::string("Movie"))
, m_genre(a_genre) {
}


Movie::Movie(const Movie& a_other)
: ArtWork(a_other){
}


Movie& Movie::operator=(const Movie &a_other) {
    this->ArtWork::operator=(a_other);
    this->m_genre = a_other.m_genre;

    return *this;
}


Movie::~Movie() {
}


const Movie::Genre& Movie::GetGenre() const {
    return this->m_genre;
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