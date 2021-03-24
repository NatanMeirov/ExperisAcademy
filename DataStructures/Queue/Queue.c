#include <stdlib.h> /* Used for size_t, malloc, calloc, free */
#include "Queue.h"
#include "ADTDefs.h"


#define MAGIC_NUM 3764901
#define IS_EMPTY 1
#define IS_NOT_EMPTY 0

struct Queue
{
	int* m_queue;
	size_t m_sizeOfQueue;
	size_t m_headIndex;
	size_t m_tailIndex;
	size_t m_numOfItems;
	int m_magicNumber;
};


/* Validation Functions Declarations: */
ADTErr ValidateSize(size_t _size);
ADTErr ValidateInitializedQueue(Queue* _queue);
ADTErr ValidateNotNullIntPtr(int* _intPtr);
ADTErr ValidateNotFullQueue(Queue* _queue);
ADTErr ValidateNotEmptyQueue(Queue* _queue);


/* Helper Functions Declarations: */
Queue* InitializeNewQueue(Queue* _queue, size_t _size);


/*---------------------------------------------------- Main API Functions: ----------------------------------------------------------------*/

Queue* QueueCreate(size_t _size)
{
	Queue* newQueue = NULL;
	ADTErr statusCode;
	
	if((ValidateSize(_size)) != ERR_OK)
	{
		return NULL;
	}
	
	newQueue = malloc(sizeof(Queue));
	
	if(ValidateInitializedQueue(newQueue) == ERR_NOT_INITIALIZED)
	{
		return NULL;
	}

	newQueue->m_queue = calloc(_size, sizeof(int));
	
	if(ValidateNotNullIntPtr(newQueue->m_queue) == ERR_NOT_INITIALIZED)
	{
		free(newQueue);
		return NULL;
	}

	return InitializeNewQueue(newQueue, _size);
}


void QueueDestroy(Queue* _queue)
{
	if(ValidateInitializedQueue(_queue) != ERR_NOT_INITIALIZED && _queue->m_magicNumber == MAGIC_NUM)
	{
		if(ValidateNotNullIntPtr(_queue->m_queue) != ERR_NOT_INITIALIZED && /* For Thread safety: */ && _queue->m_magicNumber == MAGIC_NUM)
		{
			free(_queue->m_queue);
			_queue->m_queue = NULL;	
		}
		
		_queue->m_magicNumber = 0;
		free(_queue);
	}
}


ADTErr QueueInsert(Queue* _queue, int _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedQueue(_queue)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotFullQueue(_queue)) != ERR_OK)
	{
		return statusCode;
	}
	
	_queue->m_queue[_queue->m_tailIndex] = _item;
	_queue->m_tailIndex = (_queue->m_tailIndex + 1) % _queue->m_sizeOfQueue;
	_queue->numOfItems++;
	
	return ERR_OK;
}


ADTErr QueueRemove(Queue* _queue, int* _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedQueue(_queue)) != ERR_OK || (statusCode = ValidateNotNullIntPtr(_item)) != ERR_OK)
	{
		return statusCode;
	}
	
	
	if((statusCode = ValidateNotEmptyQueue(_queue)) != ERR_OK)
	{
		return statusCode;
	}
	
	*_item = _queue->m_queue[_queue->m_headIndex];
	_queue->m_queue[_queue->m_headIndex] = 0;
	_queue->m_headIndex = (_queue->m_headIndex + 1) % _queue->m_sizeOfQueue;
	_queue->m_numOfItems--;
	
	return ERR_OK;
}


int QueueIsEmpty(Queue* _queue)
{
	if(ValidateInitializedQueue(_queue) == ERR_NOT_INITIALIZED)
	{
		return -1;
	}
		
	return (_queue->m_numOfItems == 0) ? IS_EMPTY : IS_NOT_EMPTY;
}


/*------------------------------------------------- End of Main API Functions ------------------------------------------------------------*/


/* Validation Functions: */
ADTErr ValidateSize(size_t _size)
{
	return (_size > 0) ? ERR_OK : ERR_WRONG_SIZE;
}


ADTErr ValidateInitializedQueue(Queue* _queue)
{
	return (!_queue) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullIntPtr(int* _intPtr)
{
	return (!_intPtr) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotFullQueue(Queue* _queue)
{
	return (_queue->m_sizeOfQueue == _queue->m_numOfItems) ? ERR_OVERFLOW : ERR_OK;
}


ADTErr ValidateNotEmptyQueue(Queue* _queue)
{
	return (_queue->m_numOfItems == 0) ? ERR_UNDERFLOW : ERR_OK; /* Could use the same check as the QueueIsEmpty function */
}


/* Helper Functions: */
Queue* InitializeNewQueue(Queue* _queue, size_t _size)
{
	_queue->m_sizeOfQueue = _size;
	_queue->m_headIndex = 0;
	_queue->m_tailIndex = 0;
	_queue->m_numOfItems = 0;
	_queue->m_magicNumber = MAGIC_NUM;
	
	return _queue;
}


/* Testing Functions: */
int* GetQueue(Queue* _queue)
{
	return _queue->m_queue;
}


size_t GetSizeOfQueue(Queue* _queue)
{
	return _queue->m_sizeOfQueue;
}


size_t GetHeadIndexOfQueue(Queue* _queue)
{
	return _queue->m_headIndex;
}


size_t GetTailIndexOfQueue(Queue* _queue)
{
	return _queue->m_tailIndex;
}


size_t GetNumberOfItemsInQueue(Queue* _queue)
{
	return _queue->m_numOfItems;
}


int GetMagicNumberOfQueue(Queue* _queue)
{
	return _queue->m_magicNumber;
}

