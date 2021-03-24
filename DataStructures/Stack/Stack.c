#include <stdlib.h> /* Used for size_t and malloc */
#include "ADTDefs.h"
#include "Vector.h"
#include "Stack.h"

#define IS_EMPTY 1
#define IS_NOT_EMPTY 0
#define MAGIC_NUM 7643902


struct Stack
{
	Vector* m_vector;
	int m_magicNumber;
};


/* Validation Functions Declaration: */
ADTErr ValidateNoDoubleZeroSize(size_t _size, size_t _blockSize);
ADTErr ValidateInitializedStack(Stack* _stack);
ADTErr ValidateNotNullVector(Vector* _vector);
ADTErr ValidateNotNullItemPtr(int* _itemPtr);


/*---------------------------------------------------------- Main API Functions ----------------------------------------------------------*/

Stack* StackCreate(size_t _size, size_t _blockSize)
{
	Stack* newStack = NULL;
	
	if(ValidateNoDoubleZeroSize(_size, _blockSize) != ERR_OK)
	{
		return NULL;
	}
		
	newStack = malloc(sizeof(Stack));
	
	if(ValidateInitializedStack(newStack) != ERR_OK)
	{
		return NULL;
	}
	
	newStack->m_vector = VectorCreate(_size, _blockSize);
	
	if(ValidateNotNullVector(newStack->m_vector) != ERR_OK)
	{
		free(newStack);
		return NULL;
	}
	
	newStack->m_magicNumber = MAGIC_NUM;
	
	return newStack;
}


void StackDestroy(Stack* _stack)
{
	if(ValidateInitializedStack(_stack) == ERR_OK && _stack->m_magicNumber == MAGIC_NUM)
	{
		if(ValidateNotNullVector(_stack->m_vector) == ERR_OK /* For Thread Saftey: */ && _stack->m_magicNumber == MAGIC_NUM)
		{
			VectorDestroy(_stack->m_vector);
			_stack->m_vector = NULL;
		}
		
		_stack->m_magicNumber = 0;
		free(_stack);	
	}
}


ADTErr StackPush(Stack* _stack, int _item)
{
	ADTErr statusCode;
	
	if((statusCode = ValidateInitializedStack(_stack)) != ERR_OK)
	{
		return statusCode;
	}
	
	return VectorAdd(_stack->m_vector, _item);
}


ADTErr StackPop(Stack* _stack, int* _item)
{
	ADTErr statusCode;
	size_t numOfItems;
	
	
	if((statusCode = ValidateInitializedStack(_stack)) != ERR_OK || (statusCode = ValidateNotNullItemPtr(_item)) != ERR_OK)
	{
		return statusCode;
	}
	
	return VectorDelete(_stack->m_vector, _item);
}


ADTErr StackTop(Stack* _stack, int* _item)
{
	ADTErr statusCode;
	size_t numOfItems;
	
	
	if((statusCode = ValidateInitializedStack(_stack)) != ERR_OK || (statusCode = ValidateNotNullItemPtr(_item)) != ERR_OK)
	{
		return statusCode;
	}
	
	VectorGetItemsNumber(_stack->m_vector, &numOfItems);
	
	if(numOfItems == 0)
	{
		return ERR_UNDERFLOW;
	}
	
	return VectorGet(_stack->m_vector, numOfItems - 1, _item);
}


int StackIsEmpty(Stack* _stack)
{
	ADTErr statusCode;
	size_t numOfItems;
	
	if((statusCode = ValidateInitializedStack(_stack)) != ERR_OK)
	{
		return -1; /* Error */
	}
	
	VectorGetItemsNumber(_stack->m_vector, &numOfItems);
		
	return (numOfItems == 0) ? IS_EMPTY /* true */ : IS_NOT_EMPTY; /* false */
}


/*--------------------------------------------------------- End of Main API Functions  ---------------------------------------------------*/


/* Validation Functions: */
ADTErr ValidateNoDoubleZeroSize(size_t _size, size_t _blockSize)
{
	return (_size == 0 && _blockSize == 0) ? ERR_WRONG_SIZE : ERR_OK;
}


ADTErr ValidateInitializedStack(Stack* _stack)
{
	return (!_stack) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullVector(Vector* _vector)
{
	return (!_vector) ? ERR_NOT_INITIALIZED : ERR_OK;
}


ADTErr ValidateNotNullItemPtr(int* _itemPtr)
{
	return (!_itemPtr) ? ERR_NOT_INITIALIZED : ERR_OK;
}
	
	

