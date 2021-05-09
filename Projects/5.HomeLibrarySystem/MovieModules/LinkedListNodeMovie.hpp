#ifndef __LINKEDLISTNODEMOVIE_HPP__
#define __LINKEDLISTNODEMOVIE_HPP__


#include "Movie.hpp"


class LinkedListNodeMovie {
public:

    // LinkedListNodeMovie(); - Default node construction is NOT allowed - a node must be initialized a node with a given Movie!
    LinkedListNodeMovie(const Movie& a_Movie); // Assign NULL to the pointer of the next
    LinkedListNodeMovie(const Movie& a_Movie, LinkedListNodeMovie* a_next);
    LinkedListNodeMovie(const LinkedListNodeMovie& a_other);
    LinkedListNodeMovie& operator=(const LinkedListNodeMovie& a_other);
    ~LinkedListNodeMovie();

    // Methods

    void SetNext(LinkedListNodeMovie* a_next);

    // Const Methods

    Movie GetMovie() const;
    LinkedListNodeMovie* GetNext() const;

private:

    // Members
    Movie m_movie;
    LinkedListNodeMovie* m_next;
};


#endif // __LINKEDLISTNODEMOVIE_HPP__