#include "LinkedListNodeBook.hpp"
#include <cstddef> // NULL
#include "Book.hpp"


LinkedListNodeBook::LinkedListNodeBook(const Book& a_book)
: m_book(a_book)
, m_next(NULL) {
}


LinkedListNodeBook::LinkedListNodeBook(const Book& a_book, LinkedListNodeBook* a_next)
: m_book(a_book)
, m_next(a_next) {
}


LinkedListNodeBook::LinkedListNodeBook(const LinkedListNodeBook& a_other)
: m_book(a_other.m_book)
, m_next(a_other.m_next) {
}


LinkedListNodeBook& LinkedListNodeBook::operator=(const LinkedListNodeBook& a_other) {
    this->m_book = a_other.m_book;
    this->m_next = a_other.m_next;

    return *this;
}


LinkedListNodeBook::~LinkedListNodeBook() {
}


void LinkedListNodeBook::SetNext(LinkedListNodeBook* a_next) {
    this->m_next = a_next;
}


Book LinkedListNodeBook::GetBook() const {
    return this->m_book;
}


LinkedListNodeBook* LinkedListNodeBook::GetNext() const {
    return this->m_next;
}