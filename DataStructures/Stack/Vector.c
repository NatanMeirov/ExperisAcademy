#include <stdlib.h> /* Used for size_t, malloc, calloc and realloc */
#include <stdio.h>
#include "Vector.h"


#define MAGIC_NUM 5730862
#define CHUNKS_NUMBER 2

struct Vector
{
	int* m_vectorItems;
	size_t m_originalSize; /* Original allocated space for items - to avoid shrinking the array below the original size */
	size_t m_sizeOfVector; /* Actual allocated space for items */
	size_t m_numberOfItems; /* Actual number of items in vector */
	size_t m_extensionBlockSize; /* The chunk size to be allocated when no more space is available */
	int m_magicNumber; /* A validation magic number - to avoid double free() attempts by a mistake */
};


/* Validation Function Declarations: */
ADTErr ValidateSize(size_t _size);
ADTErr ValidateInitializedVectorPtr(Vector* newVector);
ADTErr ValidateNotNullIntPtr(int* intPtr);
ADTErr ValidateNotNullSizeTPtr(size_t* sizeTPtr);
ADTErr ValidateIndex(Vector* _vector, size_t _index);
ADTErr ValidateNotOverflowAfterAdd(Vector* _vector);
ADTErr ValidateNotUnderflowAfterDelete(Vector* _vector);
ADTErr ValidateSuccessfulReAllocation(int* _itemsArray);


/* Helper Function Declaration: */
Vector* InitializeVector(Vector* _vector, size_t _initialSize, size_t _extensionBlockSize);
ADTErr TryShrinkVector(Vector* _vector);
ADTErr ShrinkVector(Vector* _vector);
ADTErr TryAddMoreMemoryToVector(Vector* _vector);
void AddNewItemToEndOfVector(Vector* _vector, int _itemToAdd);
void DeleteLastItemFromEndOfVector(Vector* _vector, int* _itemToDelete);


/********************************************************** Main API Functions: *********************************************************/
Vector* VectorCreate(size_t _initialSize, size_t _extensionBlockSize)
{
	ADTErr StatusCode;
	Vector* newVector = NULL;
	
	if(_initialSize == 0 && _extensionBlockSize == 0)
	{
		return NULL;
	}
	
	newVector = malloc(sizeof(Vector)); /* returns NULL if error */
	
	if(ValidateInitializedVectorPtr(newVector) != ERR_OK)
	{
		return NULL;
	}
	
	newVector->m_vectorItems = calloc(_initialSize, sizeof(int)); /* returns NULL if error */
		
	if(ValidateNotNullIntPtr(newVector->m_vectorItems) != ERR_OK)
	{
		free(newVector);
		return NULL;
	}
	
	return InitializeVector(newVector, _initialSize, _extensionBlockSize);
}


void VectorDestroy(Vector* _vector)
{
	if(ValidateInitializedVectorPtr(_vector) == ERR_OK && _vector->m_magicNumber == MAGIC_NUM)
	{
		if(ValidateNotNullIntPtr(_vector->m_vectorItems) == ERR_OK /* For Thread Saftey: */ && _vector->m_magicNumber == MAGIC_NUM)
		{
			free(_vector->m_vectorItems);
			_vector->m_vectorItems = NULL;
		}
		
		_vector->m_magicNumber = 0;
		free(_vector);
	}
}


ADTErr VectorAdd(Vector* _vector, int _item)
{
	ADTErr statusCode;

	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotOverflowAfterAdd(_vector)) != ERR_OK)
	{
		return statusCode;
	}
	
	if(_vector->m_numberOfItems + 1 > _vector->m_sizeOfVector)
	{
		if((statusCode = TryAddMoreMemoryToVector(_vector)) != ERR_OK)
		{
			return statusCode;
		}
		/* Else: reallocation completed successfully - ready to Add the item to the vector */
	}
		
	AddNewItemToEndOfVector(_vector, _item);
	
	return ERR_OK;
}


ADTErr VectorDelete(Vector* _vector, int* _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK || (statusCode = ValidateNotNullIntPtr(_item)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotUnderflowAfterDelete(_vector)) != ERR_OK)
	{
		return statusCode;
	}
	
	DeleteLastItemFromEndOfVector(_vector, _item);
	
	TryShrinkVector(_vector); /* Currently, need to do nothing if an error has occured while tried to reallocate memory */

	return ERR_OK;
}


ADTErr VectorGet(Vector* _vector, size_t _index, int* _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK || (statusCode = ValidateNotNullIntPtr(_item)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateIndex(_vector, _index)) != ERR_OK)
	{
		return statusCode;
	}
	
	*_item = _vector->m_vectorItems[_index];
	
	return ERR_OK;
}


ADTErr VectorSet(Vector* _vector, size_t _index, int _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateIndex(_vector, _index)) != ERR_OK)
	{
		return statusCode;
	}
	
	_vector->m_vectorItems[_index] = _item;
	
	return ERR_OK;
}


ADTErr VectorGetItemsNumber(Vector* _vector, size_t* _numOfItems)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK || (statusCode = ValidateNotNullSizeTPtr(_numOfItems)) != ERR_OK)
	{
		return statusCode;
	}
	
	*_numOfItems = _vector->m_numberOfItems;
	
	return ERR_OK;
}


ADTErr VectorUpdateExtensionBlockSize(Vector* _vector, size_t _newExtensionBlockSize)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedVectorPtr(_vector)) != ERR_OK)
	{
		return statusCode;
	}
	
	_vector->m_extensionBlockSize = _newExtensionBlockSize;
	
	return ERR_OK;
}
/********************************************************** End Of Main API Functions ****************************************************/



/* Validation Functions: */
ADTErr ValidateInitializedVectorPtr(Vector* newVector)
{
	return (!newVector) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullIntPtr(int* intPtr)
{
	return (!intPtr) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullSizeTPtr(size_t* sizeTPtr)
{
	return (!sizeTPtr) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateIndex(Vector* _vector, size_t _index)
{
	return (_index > _vector->m_numberOfItems - 1) ? ERR_WRONG_INDEX : ERR_OK;
}


ADTErr ValidateNotOverflowAfterAdd(Vector* _vector)
{
	return (_vector->m_sizeOfVector + _vector->m_extensionBlockSize < _vector->m_numberOfItems + 1) ? ERR_OVERFLOW : ERR_OK;
}


ADTErr ValidateNotUnderflowAfterDelete(Vector* _vector)
{
	return (_vector->m_numberOfItems == 0) ? ERR_UNDERFLOW : ERR_OK;
}


ADTErr ValidateSuccessfulReAllocation(int* _itemsArray)
{
	return (ValidateNotNullIntPtr(_itemsArray) == ERR_NOT_INITIALIZED) ? ERR_REALLOCATION_FAILED : ERR_OK;
}


/* Helper Functions: */
Vector* InitializeVector(Vector* _vector, size_t _initialSize, size_t _extensionBlockSize)
{
	_vector->m_originalSize = _initialSize;
	_vector->m_sizeOfVector = _initialSize;
	_vector->m_extensionBlockSize = _extensionBlockSize;
	_vector->m_numberOfItems = 0;
	_vector->m_magicNumber = MAGIC_NUM;
	
	return _vector;
}


ADTErr TryShrinkVector(Vector* _vector)
{
	if(_vector->m_sizeOfVector > _vector->m_originalSize) /* No need: (_vector->m_sizeOfVector - _vector->m_extensionBlockSize) >= _vector->m_originalSize, because the size is growing by chunks every time (and not half chunk or something)*/
	{
		if(_vector->m_sizeOfVector - _vector->m_numberOfItems /* Free places in vector */ > _vector->m_extensionBlockSize * CHUNKS_NUMBER)
		{	
			return ShrinkVector(_vector);
		}
	}
		
	return ERR_OK; /* Ok in general - Not an error... */
}


ADTErr ShrinkVector(Vector* _vector)
{
	ADTErr statusCode;
	int* temp = realloc(_vector->m_vectorItems, ((_vector->m_sizeOfVector - _vector->m_extensionBlockSize) * sizeof(int)));
	
	if((statusCode = ValidateSuccessfulReAllocation(temp)) != ERR_OK)
	{
		return statusCode;
	}
	
	_vector->m_vectorItems = temp;
	_vector->m_sizeOfVector -= _vector->m_extensionBlockSize;
	
	return ERR_OK;
}


ADTErr TryAddMoreMemoryToVector(Vector* _vector)
{
	ADTErr statusCode;
	int* temp = realloc(_vector->m_vectorItems, ((_vector->m_sizeOfVector + _vector->m_extensionBlockSize) * sizeof(int)));
	
	if((statusCode = ValidateSuccessfulReAllocation(temp)) != ERR_OK)
	{
		return statusCode;
	}
	
	_vector->m_vectorItems = temp;
	_vector->m_sizeOfVector += _vector->m_extensionBlockSize;
	
	return ERR_OK;
}


void AddNewItemToEndOfVector(Vector* _vector, int _itemToAdd)
{
	_vector->m_vectorItems[_vector->m_numberOfItems] = _itemToAdd;
	_vector->m_numberOfItems++;
}



void DeleteLastItemFromEndOfVector(Vector* _vector, int* _itemToDelete)
{
	_vector->m_numberOfItems--;
	*_itemToDelete = _vector->m_vectorItems[_vector->m_numberOfItems];
	_vector->m_vectorItems[_vector->m_numberOfItems] = 0;
}


/* Testing Functions: */
int* GetVectorItemsArray(Vector* _vector)
{
	return _vector->m_vectorItems;
}


size_t GetOriginalVectorSize(Vector* _vector)
{
	return _vector->m_originalSize;
}


size_t GetSizeOfVector(Vector* _vector)
{
	return _vector->m_sizeOfVector;
}


size_t GetNumberOfItemsInVector(Vector* _vector)
{
	return _vector->m_numberOfItems;
}


size_t GetVectorExtensionBlockSize(Vector* _vector)
{
	return _vector->m_extensionBlockSize;
}


int GetVectorMagicNumber(Vector* _vector)
{
	return _vector->m_magicNumber;
}


void SetVectorMagicNumber(Vector* _vector, int newMagicNumber)
{
	_vector->m_magicNumber = newMagicNumber;
}


void VectorPrint(Vector* _vector)
{
	int i;
	
	putchar('\n');
	putchar('[');
	for(i = 0; i < _vector->m_numberOfItems; i++)
	{
		if(i == _vector->m_numberOfItems - 1)
		{
			printf("%d", _vector->m_vectorItems[i]);
		}
		else
		{
			printf("%d, ", _vector->m_vectorItems[i]);
		}
	}
	putchar(']');
	putchar('\n');
}


void PrintAllVector(Vector* _vector)
{
	int i;
	
	putchar('\n');
	putchar('[');
	for(i = 0; i < _vector->m_sizeOfVector; i++)
	{
		if(i == _vector->m_sizeOfVector - 1)
		{
			printf("%d", _vector->m_vectorItems[i]);
		}
		else
		{
			printf("%d, ", _vector->m_vectorItems[i]);
		}
	}
	putchar(']');
	putchar('\n');
}


