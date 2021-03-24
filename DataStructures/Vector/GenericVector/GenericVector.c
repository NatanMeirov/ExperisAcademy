/**
 * @file GenericVector.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Vector data type that stores pointer to user provided elements of generic type
 * The Vector is heap allocated and can grow and shrink on demand
 * @version 0.1
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */


 /* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, realloc, free */
#include "GenericVector.h"


/* Defines: */

#define CHUNKS_NUMBER 2
#define MAX_SIZE_T ((size_t) -1) /* -1 in size_t - is the maximum size_t value */

struct Vector
{
	void** m_vectorItems;
	size_t m_originalCapacity; /* Original allocated space for items - to avoid shrinking the Vector below its original size */
	size_t m_vectorCapacity; /* Actual allocated space for items */
	size_t m_sizeOfVector; /* Actual number of items in Vector */
	size_t m_extensionBlockSize; /* The chunk size to be allocated when no more space is available */
};


/* Validation Function Declarations: */

static VectorResult ValidateInitialSize(size_t _capacity, size_t _blockSize);
static VectorResult ValidateNotNullVectorPtr(const Vector* _vector);
static VectorResult ValidateNotNullItemPtr(void** _itemPtr);
static VectorResult ValidateNotOutOfBoundsIndex(const Vector* _vector, size_t _index);
static VectorResult ValidateNotOverflowAfterAdd(const Vector* _vector);
static VectorResult ValidateNotUnderflowAfterDelete(const Vector* _vector);
static VectorResult ValidateSuccessfulReAllocationOfItemsArray(void** _itemsArray);


/* Helper Function Declaration: */

static Vector* InitializeVector(Vector* _vector, size_t _initialCapacity, size_t _extensionBlockSize);
static VectorResult TryAddMoreMemoryToVector(Vector* _vector);
static void AddNewItemToEndOfVector(Vector* _vector, void* _itemToAdd);
static VectorResult TryShrinkVector(Vector* _vector);
static VectorResult ShrinkVector(Vector* _vector);
static void DeleteLastItemFromEndOfVector(Vector* _vector, void** _itemToDelete);


/* -------------------------------------- Main API Functions ------------------------------------ */


Vector* VectorCreate(size_t _initialCapacity, size_t _extensionBlockSize)
{
	Vector* newVector = NULL;


    if(ValidateInitialSize(_initialCapacity, _extensionBlockSize) != VECTOR_SUCCESS)
	{
		return NULL;
	}

	newVector = malloc(sizeof(Vector));
	if(ValidateNotNullVectorPtr(newVector) != VECTOR_SUCCESS)
	{
		return NULL;
	}

	newVector->m_vectorItems = malloc(_initialCapacity * sizeof(void*)); /* Size of each element in the Vector will be a size of a pointer */
	if(ValidateNotNullItemPtr(newVector->m_vectorItems) != VECTOR_SUCCESS)
	{
		free(newVector);
		return NULL;
	}

	return InitializeVector(newVector, _initialCapacity, _extensionBlockSize);
}


void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
    size_t i;

	if(_vector && ValidateNotNullVectorPtr(*_vector) == VECTOR_SUCCESS)
	{
		if((*_vector)->m_vectorItems)
		{
			if(_elementDestroy) /* Pointer function is not NULL - destroy is needed for every item in the Vector */
			{
            	for(i = 0; i < (*_vector)->m_sizeOfVector; i++)
            	{
					_elementDestroy((*_vector)->m_vectorItems[i]);
            	}
			}

			free((*_vector)->m_vectorItems);
		}

		free(*_vector);
        *_vector = NULL;
	}
}


VectorResult VectorAppend(Vector* _vector, void* _item)
{
    VectorResult statusCode;

	if((statusCode = ValidateNotNullVectorPtr(_vector)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotOverflowAfterAdd(_vector)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	if(_vector->m_sizeOfVector + 1 > _vector->m_vectorCapacity)
	{
		if((statusCode = TryAddMoreMemoryToVector(_vector)) != VECTOR_SUCCESS)
		{
			return statusCode;
		}
		/* Else: reallocation completed successfully - ready to Append the item to the vector */
	}

	AddNewItemToEndOfVector(_vector, _item);

	return VECTOR_SUCCESS;
}


VectorResult VectorRemove(Vector* _vector, void** _pItem)
{
    VectorResult statusCode;

	if((statusCode = ValidateNotNullVectorPtr(_vector)) != VECTOR_SUCCESS || (statusCode = ValidateNotNullItemPtr(_pItem)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotUnderflowAfterDelete(_vector)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	DeleteLastItemFromEndOfVector(_vector, _pItem);

	TryShrinkVector(_vector); /* Currently, do nothing if an error has occurred while tried to reallocate memory */

	return VECTOR_SUCCESS;
}


VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pItem)
{
    VectorResult statusCode;

	if((statusCode = ValidateNotNullVectorPtr(_vector)) != VECTOR_SUCCESS || (statusCode = ValidateNotNullItemPtr(_pItem)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotOutOfBoundsIndex(_vector, _index)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	*_pItem = _vector->m_vectorItems[_index];

	return VECTOR_SUCCESS;
}


VectorResult VectorSet(Vector* _vector, size_t _index, void*  _itemToSet)
{
    VectorResult statusCode;

	if((statusCode = ValidateNotNullVectorPtr(_vector)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	if((statusCode = ValidateNotOutOfBoundsIndex(_vector, _index)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	_vector->m_vectorItems[_index] = _itemToSet;

	return VECTOR_SUCCESS;
}


size_t VectorSize(const Vector* _vector)
{
    if(ValidateNotNullVectorPtr(_vector) != VECTOR_SUCCESS)
    {
        return MAX_SIZE_T;
    }

    return _vector->m_sizeOfVector;
}


size_t VectorCapacity(const Vector* _vector)
{
    if(ValidateNotNullVectorPtr(_vector) != VECTOR_SUCCESS)
    {
        return MAX_SIZE_T;
    }

    return _vector->m_vectorCapacity;
}


VectorResult VectorUpdateExtensionBlockSize(Vector* _vector, size_t _newExtensionBlockSize)
{
    VectorResult statusCode;

	if((statusCode = ValidateNotNullVectorPtr(_vector)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	_vector->m_extensionBlockSize = _newExtensionBlockSize;

	return VECTOR_SUCCESS;
}

size_t VectorForEach(const Vector* _vector, VectorElementAction _actionCallbackFunction, void* _context)
{
    size_t i;

    if(ValidateNotNullVectorPtr(_vector) != VECTOR_SUCCESS || !_actionCallbackFunction)
    {
        return MAX_SIZE_T;
    }

    for(i = 0; i < _vector->m_sizeOfVector; i++)
    {
        if(_actionCallbackFunction(_vector->m_vectorItems[i], i, _context) == 0)
        {
            break;
        }
    }

    return i;
}


/* ----------------------------------- End of Main API Functions -------------------------------- */


/* Validation Functions: */

static VectorResult ValidateInitialSize(size_t _capacity, size_t _blockSize)
{
    return (_capacity == 0 && _blockSize == 0) ? VECTOR_WRONG_SIZE_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateNotNullVectorPtr(const Vector* _vector)
{
	return (!_vector) ? VECTOR_UNINITIALIZED_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateNotNullItemPtr(void** _itemPtr)
{
	return (!_itemPtr) ? VECTOR_UNINITIALIZED_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateNotOverflowAfterAdd(const Vector* _vector)
{
	return (_vector->m_vectorCapacity + _vector->m_extensionBlockSize < _vector->m_sizeOfVector + 1) ? VECTOR_OVERFLOW_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateNotUnderflowAfterDelete(const Vector* _vector)
{
	return (_vector->m_sizeOfVector == 0) ? VECTOR_UNDERFLOW_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateSuccessfulReAllocationOfItemsArray(void** _itemsArray)
{
	return (ValidateNotNullItemPtr(_itemsArray) == VECTOR_UNINITIALIZED_ERROR) ? VECTOR_ALLOCATION_ERROR : VECTOR_SUCCESS;
}


static VectorResult ValidateNotOutOfBoundsIndex(const Vector* _vector, size_t _index)
{
	return (_index > _vector->m_sizeOfVector - 1) ? VECTOR_INDEX_OUT_OF_BOUNDS_ERROR : VECTOR_SUCCESS;
}


/* Helper Functions: */

static Vector* InitializeVector(Vector* _vector, size_t _initialCapacity, size_t _extensionBlockSize)
{
	_vector->m_originalCapacity = _initialCapacity;
	_vector->m_vectorCapacity = _initialCapacity;
	_vector->m_extensionBlockSize = _extensionBlockSize;
	_vector->m_sizeOfVector = 0;

	return _vector;
}


static VectorResult TryAddMoreMemoryToVector(Vector* _vector)
{
	VectorResult statusCode;
	void** temp = realloc(_vector->m_vectorItems, ((_vector->m_vectorCapacity + _vector->m_extensionBlockSize) * sizeof(void*)));

	if((statusCode = ValidateSuccessfulReAllocationOfItemsArray(temp)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	_vector->m_vectorItems = temp;
	_vector->m_vectorCapacity += _vector->m_extensionBlockSize;

	return VECTOR_SUCCESS;
}


static void AddNewItemToEndOfVector(Vector* _vector, void* _itemToAdd)
{
	_vector->m_vectorItems[_vector->m_sizeOfVector] = _itemToAdd;
	_vector->m_sizeOfVector++;
}


static VectorResult TryShrinkVector(Vector* _vector)
{
	if(_vector->m_vectorCapacity > _vector->m_originalCapacity) /* No need: (_vector->m_vectorCapacity - _vector->m_extensionBlockSize) >= _vector->m_originalCapacity, because the size is growing by chunks every time (and not half chunk or something)*/
	{
		if(_vector->m_vectorCapacity - _vector->m_sizeOfVector /* Free places in vector */ > _vector->m_extensionBlockSize * CHUNKS_NUMBER)
		{
			return ShrinkVector(_vector);
		}
	}

	return VECTOR_SUCCESS; /* Ok in general - Not an error */
}


static VectorResult ShrinkVector(Vector* _vector)
{
	VectorResult statusCode;
	void** temp = realloc(_vector->m_vectorItems, ((_vector->m_vectorCapacity - _vector->m_extensionBlockSize) * sizeof(void*)));

	if((statusCode = ValidateSuccessfulReAllocationOfItemsArray(temp)) != VECTOR_SUCCESS)
	{
		return statusCode;
	}

	_vector->m_vectorItems = temp;
	_vector->m_vectorCapacity -= _vector->m_extensionBlockSize;

	return VECTOR_SUCCESS;
}


static void DeleteLastItemFromEndOfVector(Vector* _vector, void** _itemToDelete)
{
	_vector->m_sizeOfVector--;
	*_itemToDelete = _vector->m_vectorItems[_vector->m_sizeOfVector];
	_vector->m_vectorItems[_vector->m_sizeOfVector] = NULL;
}
