#ifndef __STRINGSLINKEDLISTNODE_HPP__
#define __STRINGSLINKEDLISTNODE_HPP__


#include "String.hpp"


class StringsLinkedListNode {
public:

    // StringsLinkedListNode(); - Default node construction is NOT allowed - a node must be initialized a node with a given string!
    StringsLinkedListNode(const String& a_string); // Assign NULL to the pointer of the next
    StringsLinkedListNode(const String& a_string, StringsLinkedListNode* a_next);
    StringsLinkedListNode(const StringsLinkedListNode& a_other);
    StringsLinkedListNode& operator=(const StringsLinkedListNode& a_other);
    ~StringsLinkedListNode();

    // Methods

    void SetNext(StringsLinkedListNode* a_next);

    // Const Methods

    String GetString() const;
    StringsLinkedListNode* GetNext() const;

private:

    // Members
    String m_string;
    StringsLinkedListNode* m_next;
};


#endif // __STRINGSLINKEDLISTNODE_HPP__