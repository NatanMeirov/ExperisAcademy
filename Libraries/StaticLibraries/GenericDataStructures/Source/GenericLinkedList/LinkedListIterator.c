/**
 * @file LinkedListIterator.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Adds an Iterator functionality to the Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * @version 1.0
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* NULL */
#include "../../Include/GenericLinkedList/LinkedListInternal.h"
#include "../../Include/GenericLinkedList/GenericLinkedList.h"
#include "../../Include/GenericLinkedList/LinkedListIterator.h"


/*---------------------------------------------- Main API Functions -----------------------------------------------*/

LinkedListIterator LinkedListIteratorBegin(const LinkedList* _list)
{
    if(!_list)
    {
        return NULL;
    }

    return (LinkedListIterator)_list->m_head.m_next;
}


LinkedListIterator LinkedListIteratorEnd(const LinkedList* _list)
{
    if(!_list)
    {
        return NULL;
    }

    return (LinkedListIterator)&(_list->m_tail);
}


int LinkedListIteratorEquals(const LinkedListIterator _firstIterator, const LinkedListIterator _secondIterator)
{
    return _firstIterator == _secondIterator; /* Works even if one of them is NULL */
}


LinkedListIterator LinkedListIteratorNext(LinkedListIterator _iterator)
{
    if(!_iterator)
    {
        return NULL;
    }

    return (LinkedListIterator)((Node*)_iterator)->m_next; /* No need to check if reached to LinkedList's end, because of the use of the tail's self pointing (next is tail too) */
}


LinkedListIterator LinkedListIteratorPrev(LinkedListIterator _iterator)
{
    if(!_iterator)
    {
        return NULL;
    }

    return (LinkedListIterator)((Node*)_iterator)->m_prev; /* No need to check if reached to LinkedList's begin, because of the use of the head's self pointing (prev is head too) */
}


void* LinkedListIteratorGet(LinkedListIterator _iterator)
{
    if(!_iterator)
    {
        return NULL;
    }

    return ((Node*)_iterator)->m_element;
}


void* LinkedListIteratorSet(LinkedListIterator _iterator, void* _element)
{
    void* tempElement = NULL;

    if(!_iterator)
    {
        return NULL;
    }

    tempElement = ((Node*)_iterator)->m_element;
    ((Node*)_iterator)->m_element = _element;

    return tempElement;
}


LinkedListIterator LinkedListIteratorInsertBefore(LinkedListIterator _iterator, void* _element)
{
    Node* node = NULL;

    if(!_iterator)
    {
        return NULL;
    }

    node = CreateNewNode(_element);
    if(!node)
    {
        return NULL;
    }

    InsertNodeToLinkedList(node, (Node*)_iterator);

    return (LinkedListIterator)((Node*)_iterator)->m_prev; /* The prev Node is the newly inserted Node */
}


void* LinkedListIteratorRemove(LinkedListIterator _iterator)
{
    void* tempElement = NULL;

    if(!_iterator)
    {
        return NULL;
    }

    tempElement = ((Node*)_iterator)->m_element;
    RemoveNodeFromLinkedList((Node*)_iterator);
    DestroySingleNode((Node*)_iterator, NULL);

    return tempElement;
}

/*------------------------------------------- End of Main API Functions -------------------------------------------*/
