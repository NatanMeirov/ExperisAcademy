/**
 * @file LinkedListIterator.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Adds an Iterator functionality support to the Generic Double Linked List data type that stores pointer to user provided elements of generic type
 * @version 1.0
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __LINKEDLISTITERATOR_H__
#define __LINKEDLISTITERATOR_H__

/* Includes: */

#include "LinkedListInternal.h"


/* Defines: */

typedef void* LinkedListIterator;


/* ------------------------------------------- Main API Functions ------------------------------------ */

/** 
 * @brief Gets an Iterator to the given LinkedList's begin
 * @details Time complexity: O(1)
 * @param[in] _list: A list to return its begin Iterator, pointing at the first element of the LinkedList if not empty,
 *                   or at the tail sentinel of the LinkedList if empty
 * @return LinkedListIterator - An Iterator pointing at the LinkedList's beginning
 */
LinkedListIterator LinkedListIteratorBegin(const LinkedList* _list);


/** 
 * @brief Gets an Iterator to the given LinkedList's end
 * @details Time complexity: O(1)
 * @param[in] _list: A list to return its end Iterator, pointing at the tail sentinel of the LinkedList
 * @return LinkedListIterator - An Iterator pointing at the LinkedList's ending
 */
LinkedListIterator LinkedListIteratorEnd(const LinkedList* _list);


/** 
 * @brief Checks if two given Iterators are equal or not
 * @details Time complexity: O(1)
 * @param[in] _firstIterator: The first Iterator
 * @param[in] _secondIterator: The second Iterator
 * @return int - 0 if the Iterators are not equal / 1 if the Iterators are equal
 */
int LinkedListIteratorEquals(const LinkedListIterator _firstIterator, const LinkedListIterator _secondIterator);


/** 
 * @brief Gets the next element's Iterator of a given Iterator
 * @details Time complexity: O(1)
 * @param[in] _iterator: An Iterator to get its next element's Iterator
 * @return LinkedListIterator - An Iterator pointing at the next element
 * @warning if _iterator is the end of the LinkedList: current Iterator (end) will be returned
 */
LinkedListIterator LinkedListIteratorNext(LinkedListIterator _iterator);


/** 
 * @brief Gets the previous element's Iterator of a given Iterator
 * @details Time complexity: O(1)
 * @param[in] _iterator: An Iterator to get its previous element's Iterator
 * @return LinkedListIterator - An Iterator pointing at the previous element
 * @warning if _iterator is the begin of the LinkedList: current Iterator (begin) will be returned
 */
LinkedListIterator LinkedListIteratorPrev(LinkedListIterator _iterator);


/** 
 * @brief Gets the element that the given Iterator is pointing to
 * @details Time complexity: O(1)
 * @param[in] _iterator : An Iterator to get its pointed element
 * @return void* - The element that the Iterator is pointing at, on success / NULL - on failure
 * @warning if _iterator is the end of the LinkedList: function will fail and return NULL
 */
 void* LinkedListIteratorGet(LinkedListIterator _iterator);


/** 
 * @brief Sets a new element to replace the current element that the given Iterator is pointing to
 * @details Time complexity: O(1)
 * @param[in] _iterator : An Iterator to set a new value to its pointed element
 * @return void* - The previous (replaced) element that the Iterator was pointing at, on success / NULL - on failure
 * @warning if _iterator is the end of the LinkedList: function will fail and return NULL
 */
void* LinkedListIteratorSet(LinkedListIterator _iterator, void* _element);


/** 
 * @brief Inserts a new element before the element that the given Iterator is pointing to
 * @details Time complexity: O(1)
 * @param[in] _iterator : An Iterator to insert a new element one place before its pointed element
 * @return LinkedListIterator - An Iterator pointing to the newly inserted element, on success / NULL - on failure
 */
LinkedListIterator LinkedListIteratorInsertBefore(LinkedListIterator _iterator, void* _element);


/** 
 * @brief Removes an element that the given Iterator is pointing to
 * @details Time complexity: O(1)
 * @param[in] _iterator : An Iterator to remove its pointed element
 * @return void* - The removed element that the Iterator was pointing at, on success / NULL - on failure
 */
void* LinkedListIteratorRemove(LinkedListIterator _iterator);

/* --------------------------------------- End of Main API Functions ---------------------------------- */

#endif /* #ifndef __LINKEDLISTITERATOR_H__ */