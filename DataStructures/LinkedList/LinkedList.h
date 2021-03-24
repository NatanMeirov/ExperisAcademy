/* Linked List header file */
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdlib.h> /* Used for size_t */
#include "ADTDefs.h"


typedef struct LinkedList LinkedList;


/*
- Description: Creates and returns a pointer to a newly created LinkedList

- Input: None

- Output: pointer to LinkedList, else NULL is error has occured

- Errors: ERR_ALLOCATION_FAILED (if failed to allocate new memory)
*/
LinkedList* LinkedListCreate(void);


/*
- Description: Destroys a given LinkedList (releasing memory)

- Input: [_list:] a pointer to a LinkedList to destroy

- Output: None

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL)
*/
void LinkedListDestroy(LinkedList* _list);


/*
- Description: Inserts a new data item to be the head of the LinkedList (inserts it to the first place)

- Input: [_list:] a pointer to a LinkedList, [_data:] a data item to insert as the new head of the LinkedList

- Output: (ADTErr status code:) ERR_OK if succeed, else error 

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_ALLOCATION_FAILED (if failed to allocate memory for the new data item)
*/
ADTErr LinkedListInsertHead(LinkedList* _list, int _data);


/*
- Description: Inserts a new data item to be the tail of the LinkedList (inserts it to the last place)

- Input: [_list:] a pointer to a LinkedList, [_data:] a data item to insert as the new tail of the LinkedList

- Output: (ADTErr status code:) ERR_OK if succeed, else error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_ALLOCATION_FAILED (if failed to allocate memory for the new data item)
*/
ADTErr LinkedListInsertTail(LinkedList* _list, int _data);


/*
- Description: Removes a data item from the head (start) of the LinkedList, and returning its value by reference (via int pointer)

- Input: [_list:] a pointer to a LinkedList, [_data:] an int pointer to item, to return the removed value from the LinkedList by reference

- Output: (ADTErr status code:) ERR_OK if succeed, else error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_UNDERFLOW (if the LinkedList is empty and there are no item to remove)
*/
ADTErr LinkedListRemoveHead(LinkedList* _list, int* _data);


/*
- Description: Removes a data item from the tail (end) of the LinkedList, and returning its value by reference (via int pointer)

- Input: [_list:] a pointer to a LinkedList, [_data:] an int pointer to item, to return the removed value from the LinkedList by reference

- Output: (ADTErr status code:) ERR_OK if succeed, else error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_UNDERFLOW (if the LinkedList is empty and there are no item to remove)
*/
ADTErr LinkedListRemoveTail(LinkedList* _list, int* _data);


/*
- Description: Counts and returns the data items in the LinkedList

- Input: [_list:] a pointer to a LinkedList

- Output: Data items count

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL)
*/
size_t LinkedListCountItems(LinkedList* _list);


/*
- Description: Checks if a given LinkedList is empty or not - 0 if not empty, 1 if empty, else -1 if error

- Input: [_list:] a pointer to a LinkedList 

- Output: 0 if not empty, 1 if empty, else -1 if error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL)
*/
int LinkedListIsEmpty(LinkedList* _list);


#endif /* #ifndef __LINKEDLIST_H__ */
