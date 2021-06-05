#ifndef __STRINGSLINKEDLIST_HPP__
#define __STRINGSLINKEDLIST_HPP__


#include "String.hpp"
#include "StringsLinkedListNode.hpp"


class StringsLinkedList {
public:

    StringsLinkedList();
    StringsLinkedList(const StringsLinkedList& a_other);
    StringsLinkedList& operator=(const StringsLinkedList& a_other);
    ~StringsLinkedList();

    // Methods
    void PushHead(const String& a_string);
    void PushTail(const String& a_string);
    String PopHead();
    String PopTail();

    // Const Methods
    String GetHead() const;
    String GetTail() const;

private:

    // Members
    StringsLinkedListNode* m_head;
};



# endif // __STRINGSLINKEDLIST_HPP__