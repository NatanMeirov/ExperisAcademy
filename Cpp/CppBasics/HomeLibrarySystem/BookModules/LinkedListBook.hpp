#ifndef __LINKEDLISTBOOK_HPP__
#define __LINKEDLISTBOOK_HPP__

#include <cstddef> // size_t
#include "Book.hpp"
#include "LinkedListNodeBook.hpp"


class LinkedListBook {
public:

    LinkedListBook();
    LinkedListBook(const LinkedListBook& a_other);
    LinkedListBook& operator=(const LinkedListBook& a_other);
    ~LinkedListBook();

    // Methods
    void PushHead(const Book& a_book);
    void PushTail(const Book& a_book);
    Book PopHead();
    Book PopTail();

    // Const Methods
    Book GetHead() const;
    Book GetTail() const;
    Book GetBookAt(size_t a_index) const; // index starts at 1..n
    const size_t Size() const; // Complexity: O(1) [instead of O(n)]

private:

    // Members
    LinkedListNodeBook* m_head;
    size_t m_booksNumber;
};


# endif // __LINKEDLISTBOOK_HPP__