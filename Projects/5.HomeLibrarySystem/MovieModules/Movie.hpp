#ifndef __MOVIE_HPP__
#define __MOVIE_HPP__


class Movie {
public:

    enum Genre {
        ACTION,
        COMEDY,
        DRAMA,
        ANIME,
        OTHER_GENRE
    };

    static const int MOVIE_MIN_RELEASED_YEAR = 1900;

    Movie(const char* a_movieName, const char* a_director, Genre a_genre, unsigned int a_releasedYear, unsigned int a_receivedYear);
    Movie(const Movie& a_otherMovie);
    Movie& operator=(const Movie& a_otherMovie);
    ~Movie();


    const char* GetMovieName() const;
    const char* GetDirector() const;
    const Genre GetGenre() const;
    const unsigned int GetReleasedYear() const;
    const unsigned int GetReceivedYear() const;
    const char* GetGenreAsString() const;


private:

    char* m_movieName;
    char* m_director;
    Genre m_genre;
    unsigned int m_releasedYear;
    unsigned int m_receivedYear;
};


#endif // __MOVIE_HPP__