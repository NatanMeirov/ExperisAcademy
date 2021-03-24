/**
 * @file GenericQueue.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Queue data type that stores pointer to user provided elements of generic type
 * @version 0.1
 * @date 2021-03-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */


 /* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, realloc, free */
#include "GenericQueue.h"


/* Defines: */

#define IS_EMPTY 1
#define IS_NOT_EMPTY 0
#define MAX_SIZE_T ((size_t) -1) /* -1 in size_t - is the maximum size_t value */

struct Queue
{
    void** m_queueItems;
	size_t m_sizeOfQueue;
	size_t m_headIndex;
	size_t m_tailIndex;
	size_t m_itemsNumInQueue;
};


/* Validation Function Declarations: */

static QueueResult ValidateNotNullQueuePtr(const Queue* _queue);
static QueueResult ValidateNotNullItemPtr(void** _itemPtr);
static QueueResult ValidateNotNullData(void* _data);
static QueueResult ValidateNotFullQueue(const Queue* _queue);
static QueueResult ValidateNotEmptyQueue(const Queue* _queue);


/* Helper Function Declaration: */

static Queue* InitializeNewQueue(Queue* _queue, size_t _sizeOfQueue);


/* -------------------------------------- Main API Functions ------------------------------------ */


Queue* QueueCreate(size_t _initialSizeOfQueue)
{
	Queue* newQueue = NULL;

	if(_initialSizeOfQueue == 0)
	{
		return NULL;
	}

	newQueue = malloc(sizeof(Queue));

	if(ValidateNotNullQueuePtr(newQueue) != QUEUE_SUCCESS)
	{
		return NULL;
	}

	newQueue->m_queueItems = calloc(_initialSizeOfQueue, sizeof(void*)); /* Initialize the whole queue will NULLs */

	if(ValidateNotNullItemPtr(newQueue->m_queueItems) != QUEUE_SUCCESS)
	{
		free(newQueue);
		return NULL;
	}

	return InitializeNewQueue(newQueue, _initialSizeOfQueue);
}


void QueueDestroy(Queue** _queue, DestroyItemFunc _destroyItemFunc)
{
    size_t i, currentItemIndex;

	if(_queue && ValidateNotNullQueuePtr(*_queue) == QUEUE_SUCCESS)
	{
		if((*_queue)->m_queueItems)
		{
            if(_destroyItemFunc) /* Pointer function is not NULL - destroy is needed for every item in the Queue */
			{
            	for(currentItemIndex = (*_queue)->m_headIndex ,i = 0; i < (*_queue)->m_itemsNumInQueue; i++, currentItemIndex = (currentItemIndex + 1) % (*_queue)->m_sizeOfQueue)
            	{
					_destroyItemFunc((*_queue)->m_queueItems[currentItemIndex]);
            	}
			}

			free((*_queue)->m_queueItems);
		}

		free(*_queue);
        *_queue = NULL;
	}
}


QueueResult QueueInsert(Queue* _queue, void* _itemToInsert)
{
    QueueResult statusCode;

	if((statusCode = ValidateNotNullQueuePtr(_queue)) != QUEUE_SUCCESS || (statusCode = ValidateNotNullData(_itemToInsert)) != QUEUE_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotFullQueue(_queue)) != QUEUE_SUCCESS)
	{
		return statusCode;
	}

	_queue->m_queueItems[_queue->m_tailIndex] = _itemToInsert;
	_queue->m_tailIndex = (_queue->m_tailIndex + 1) % _queue->m_sizeOfQueue;
	_queue->m_itemsNumInQueue++;

	return QUEUE_SUCCESS;
}


QueueResult QueueRemove(Queue* _queue, void** _pItem)
{
	QueueResult statusCode;

	if((statusCode = ValidateNotNullQueuePtr(_queue)) != QUEUE_SUCCESS || (statusCode = ValidateNotNullItemPtr(_pItem)) != QUEUE_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotEmptyQueue(_queue)) != QUEUE_SUCCESS)
	{
		return statusCode;
	}

	*_pItem = _queue->m_queueItems[_queue->m_headIndex];
	_queue->m_queueItems[_queue->m_headIndex] = NULL;
	_queue->m_headIndex = (_queue->m_headIndex + 1) % _queue->m_sizeOfQueue;
	_queue->m_itemsNumInQueue--;

	return QUEUE_SUCCESS;
}


int QueueIsEmpty(Queue* _queue)
{
	if(ValidateNotNullQueuePtr(_queue) != QUEUE_SUCCESS)
	{
		return -1;
	}

	return (_queue->m_itemsNumInQueue == 0) ? IS_EMPTY : IS_NOT_EMPTY;
}


size_t QueueForEach(Queue* _queue, ActionFunction _actionCallbackFunction, void* _context)
{
    size_t i, currentItemIndex;

    if(ValidateNotNullQueuePtr(_queue) != QUEUE_SUCCESS || !_actionCallbackFunction)
    {
        return MAX_SIZE_T;
    }

    for(currentItemIndex = _queue->m_headIndex ,i = 1; i <= _queue->m_itemsNumInQueue; i++, currentItemIndex = (currentItemIndex + 1) % _queue->m_sizeOfQueue)
    {
        if(_actionCallbackFunction(_queue->m_queueItems[currentItemIndex], _context) == 0)
        {
            break;
        }
    }

    if(i == _queue->m_itemsNumInQueue + 1) /* Successfully iterated all the Queue - action was invoked {itemsNumInQueue} times */
    {
        i--; /* Decrementing i to be equal to {itemsNumInQueue} */
    }

    return i;
}


/* ----------------------------------- End of Main API Functions -------------------------------- */


/* Validation Functions: */

static QueueResult ValidateNotNullQueuePtr(const Queue* _queue)
{
	return (!_queue) ? QUEUE_UNINITIALIZED_ERROR : QUEUE_SUCCESS;
}


static QueueResult ValidateNotNullItemPtr(void** _itemPtr)
{
	return (!_itemPtr) ? QUEUE_UNINITIALIZED_ERROR : QUEUE_SUCCESS;
}


static QueueResult ValidateNotNullData(void* _data)
{
    return (!_data) ? QUEUE_DATA_UNINITIALIZED_ERROR : QUEUE_SUCCESS;
}


static QueueResult ValidateNotFullQueue(const Queue* _queue)
{
	return (_queue->m_sizeOfQueue == _queue->m_itemsNumInQueue) ? QUEUE_OVERFLOW_ERROR : QUEUE_SUCCESS;
}


static QueueResult ValidateNotEmptyQueue(const Queue* _queue)
{
	return (_queue->m_itemsNumInQueue == 0) ? QUEUE_UNDERFLOW_ERROR : QUEUE_SUCCESS; /* Could use the same check as the QueueIsEmpty function */
}


/* Helper Functions: */

static Queue* InitializeNewQueue(Queue* _queue, size_t _sizeOfQueue)
{
	_queue->m_sizeOfQueue = _sizeOfQueue;
	_queue->m_headIndex = 0;
	_queue->m_tailIndex = 0;
	_queue->m_itemsNumInQueue = 0;

	return _queue;
}
