#ifndef __LINKEDLISTNODEBOOK_HPP__
#define __LINKEDLISTNODEBOOK_HPP__


#include "Book.hpp"


class LinkedListNodeBook {
public:

    // LinkedListNodeBook(); - Default node construction is NOT allowed - a node must be initialized a node with a given Book!
    LinkedListNodeBook(const Book& a_Book); // Assign NULL to the pointer of the next
    LinkedListNodeBook(const Book& a_Book, LinkedListNodeBook* a_next);
    LinkedListNodeBook(const LinkedListNodeBook& a_other);
    LinkedListNodeBook& operator=(const LinkedListNodeBook& a_other);
    ~LinkedListNodeBook();

    // Methods

    void SetNext(LinkedListNodeBook* a_next);

    // Const Methods

    Book GetBook() const;
    LinkedListNodeBook* GetNext() const;

private:

    // Members
    Book m_book;
    LinkedListNodeBook* m_next;
};


#endif // __LINKEDLISTNODEBOOK_HPP__