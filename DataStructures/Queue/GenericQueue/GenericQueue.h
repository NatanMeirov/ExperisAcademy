/**
 * @file GenericQueue.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Queue data type that stores pointer to user provided elements of generic type
 * @version 0.1
 * @date 2021-03-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __GENERICQUEUE_H__
#define __GENERICQUEUE_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

typedef enum QueueResult
{
    QUEUE_SUCCESS,
    QUEUE_UNINITIALIZED_ERROR,
    QUEUE_OVERFLOW_ERROR,
    QUEUE_UNDERFLOW_ERROR,
    QUEUE_DATA_UNINITIALIZED_ERROR
} QueueResult;

typedef struct Queue Queue;

/**
 * @brief An action callback function that will be called on each element of the queue when destroying the Queue
 * @param[in] _element: A pointer to an element to destroy
 * @return None
 */
typedef void (*DestroyItemFunc)(void* _element);

/**
 * @brief An action callback function that will be called for each element
 * @param[in] _element: A pointer to an element
 * @param[in] _context: A pointer to the context to pass to the action callback function, when the function is called
 * @return int - 0, if the iteration should stop on the current element / 1, if the iteration should continue to the next element
 */
typedef int (*ActionFunction)(const void* _element, void* _context);


/* ------------------------------------------- Main API Functions ------------------------------------ */

/**
 * @brief Dynamically creates a new Queue object of a given size
 * @param[in] _initialSizeOfQueue: The size of the Queue to create
 * @return Queue* - on success / NULL - on failure
 * @warning The Queue has fixed size, and does not have the ability to grow and shrink on demand
 * @warning If _initialSizeOfQueue is 0: function will fail and return NULL
 */
Queue* QueueCreate(size_t _initialSizeOfQueue);


/**
 * @brief Dynamically deallocates a previously allocated Queue, NULLs the Queue's pointer
 * @param[in] _queue: A Queue to deallocate
 * @param[in] _destroyItemFunc: A function pointer to be used to destroy each element in the given Queue,
 * 			 				   or a NULL if no such destroy is required
 * @return None
 *
 */
void QueueDestroy(Queue** _queue, DestroyItemFunc _destroyItemFunc);


/**
 * @brief Inserts an item to the end of the Queue, if the Queue is not full
 * @param[in] _queue: A Queue to insert an item to
 * @param[in] _itemToInsert: The item to insert to the end of the Queue
 * @return QueueResult - success or error status code
 * @retval QUEUE_SUCCESS on success
 * @retval QUEUE_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval QUEUE_DATA_UNINITIALIZED_ERROR on error - given item to insert is NULL
 * @retval QUEUE_OVERFLOW_ERROR on error - reached size limit, no more room to add another item
 *
 */
QueueResult QueueInsert(Queue* _queue, void* _itemToInsert);


/**
 * @brief Removes an item from the beginning of the Queue ,if the Queue is not empty
 * @param[in] _queue: A Queue to remove an item from
 * @param[in] _item: A pointer to a variable that used to return the wanted item value by reference
 * @return QueueResult - success or error status code
 * @retval QUEUE_SUCCESS on success
 * @retval QUEUE_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval QUEUE_UNDERFLOW_ERROR on error - queue is empty, no more items to remove
 * @warning If _pItem is NULL: function will fail and return QUEUE_UNINITIALIZED_ERROR
 */
QueueResult QueueRemove(Queue* _queue, void** _pItem);


/**
 * @brief Checks if a given Queue is empty or not
 * @param[in] _queue: A Queue to check if is empty
 * @return int - 0 if Queue is not empty or 1 if Queue is empty, on success / -1, on failure
 */
int QueueIsEmpty(Queue* _queue);


/**
 * @brief Iterates over all the elements in the Queue, and triggers an action callback function on every single element
 * @details The user provides an action callback function that will be called for each element,
 *          if the action callback function returns 0 for an element - the iteration will stop,
 *			if the action callback function returns 1 for an element - the iteration will continue
 * @param[in] _queue: Queue to iterate over
 * @param[in] _actionCallbackFunction: User provided action callback function pointer to be invoked for each element
 * @param[in] _context: User provided context, that will be sent to action callback function
 * @return size_t - number of times the user function was invoked (if successfully iterated all the Queue items [ == number of items in the queue]), on success
			/ MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t QueueForEach(Queue* _queue, ActionFunction _actionCallbackFunction, void* _context);

/* --------------------------------------- End of Main API Functions ---------------------------------- */

#endif /* #ifndef __GENERICQUEUE_H__ */
