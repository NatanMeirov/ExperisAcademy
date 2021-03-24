/* Queue header file */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h> /* Used for size_t */
#include "ADTDefs.h"

typedef struct Queue Queue


/*
- Description: Creates and returns a pointer to a newly created Queue

- Input: [_size:] size of the new queue (minimum queue size: 1)

- Output: A pointer to a new Queue if ERR_OK, else NULL if error

- Errors: ERR_ALLOCATION_FAILED (could not allocate memory), ERR_WRONG_SIZE (size is 0)
*/
Queue* QueueCreate(size_t _size);


/*
- Description: Destroys a given Queue (releasing its memory), validates not using double free

- Input: [_queue:] a pointer to a queue to destroy

- Output: No output

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL)
*/
void QueueDestroy(Queue* _queue);

/*
- Description: Inserts an item to the end of the queue, if the queue is not full

- Input: [_queue:] a pointer to a queue, [_item:] an item to insert to the end of the queue

- Output: [ADTErr status code:] ERR_OK if the item inserted successfully, else error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_OVERFLOW (if the queue is full) 
*/
ADTErr QueueInsert(Queue* _queue, int _item);


/*
- Description: Removes an item from the beginning of the queue if the queue is not empty, and stores the item in a pointer to an address

- Input: [_queue:] a pointer to a queue, [_item:] a pointer to an address to store the removed item

- Output: [ADTErr status code:] ERR_OK if the item removed successfully, else error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_UNDERFLOW (if queue is empty already)
*/
ADTErr QueueRemove(Queue* _queue, int* _item);


/*
- Description: Checks if a queue is empty or not, returns 1 if the queue is empty, 0 if the queue is not empty, else -1 if error has occured

- Input: [_queue:] a pointer to a queue

- Output: 1 if queue is empty, 0 if queue is not empty, -1 if error

- Errors: ERR_NOT_INITIALIZED (if pointer is NULL)
*/
int QueueIsEmpty(Queue* _queue);


#endif /* #ifndef __QUEUE_H__ */
