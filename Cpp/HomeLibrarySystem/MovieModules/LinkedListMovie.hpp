#ifndef __LINKEDLISTMOVIE_HPP__
#define __LINKEDLISTMOVIE_HPP__

#include <cstddef> // size_t
#include "Movie.hpp"
#include "LinkedListNodeMovie.hpp"


class LinkedListMovie {
public:

    LinkedListMovie();
    LinkedListMovie(const LinkedListMovie& a_other);
    LinkedListMovie& operator=(const LinkedListMovie& a_other);
    ~LinkedListMovie();

    // Methods
    void PushHead(const Movie& a_movie);
    void PushTail(const Movie& a_movie);
    Movie PopHead();
    Movie PopTail();

    // Const Methods
    Movie GetHead() const;
    Movie GetTail() const;
    Movie GetMovieAt(size_t a_index) const; // index starts at 1..n
    const size_t Size() const; // Complexity: O(1) [instead of O(n)]

private:

    // Members
    LinkedListNodeMovie* m_head;
    size_t m_moviesNumber;
};


# endif // __LINKEDLISTMOVIE_HPP__