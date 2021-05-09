#include "LinkedListNodeMovie.hpp"
#include <cstddef> // NULL
#include "Movie.hpp"


LinkedListNodeMovie::LinkedListNodeMovie(const Movie& a_movie)
: m_movie(a_movie)
, m_next(NULL) {
}


LinkedListNodeMovie::LinkedListNodeMovie(const Movie& a_movie, LinkedListNodeMovie* a_next)
: m_movie(a_movie)
, m_next(a_next) {
}


LinkedListNodeMovie::LinkedListNodeMovie(const LinkedListNodeMovie& a_other)
: m_movie(a_other.m_movie)
, m_next(a_other.m_next) {
}


LinkedListNodeMovie& LinkedListNodeMovie::operator=(const LinkedListNodeMovie& a_other) {
    this->m_movie = a_other.m_movie;
    this->m_next = a_other.m_next;

    return *this;
}


LinkedListNodeMovie::~LinkedListNodeMovie() {
}


void LinkedListNodeMovie::SetNext(LinkedListNodeMovie* a_next) {
    this->m_next = a_next;
}


Movie LinkedListNodeMovie::GetMovie() const {
    return this->m_movie;
}


LinkedListNodeMovie* LinkedListNodeMovie::GetNext() const {
    return this->m_next;
}