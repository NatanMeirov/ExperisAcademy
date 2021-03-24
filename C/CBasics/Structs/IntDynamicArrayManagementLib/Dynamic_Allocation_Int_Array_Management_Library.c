/* Libraries: */
#include <stdlib.h> /* Using malloc/calloc from stdlib.h */
#include "Dynamic_Allocation_Int_Array_Management_Library.h"

#define MAGIC 8173649 /* Stamp for safety against double free() */

/* Validation Functions Declarations: */
int ValidateSize(int _size);
int ValidateNotNullIntPtr(int* _intPtr);
int ValidateNotNullIntDynamicArrayPtr(IntDynamicArray* _intDynamicArray);
int ValidateSuccessfulAllocation(int* _intArrayPtr);
int ValidateNotMemoryOverflowAfterInsert(IntDynamicArray* _intDynamicArray);
int ValidateNotMemoryUnderflowAfterRemove(IntDynamicArray* _intDynamicArray);

/* Helper Functions Declarations: */
IntDynamicArray* InitializeIntDynamicArray(IntDynamicArray* _intDynamicArray, int _sizeOfArray, int _BlockSizeValueToIncrement);
int CreationCompleteValidationCheckOnParams(int _sizeOfArray, int _BlockSizeValueToIncrement);
int InsertionCompleteValidationCheckOnParams(IntDynamicArray* _intDynamicArray);
int RemoveCompleteValidationCheckOnParams(IntDynamicArray* _intDynamicArray, int* _removedData);
void PushNewElementToDynamicArray(IntDynamicArray* _intDynamicArray, int _dataToInsert);
int TryReallocateMemoryToDynamicArray(IntDynamicArray* _intDynamicArray);
void PopLastElementFromDynamicArray(IntDynamicArray* _intDynamicArray, int* _removedData);


/* Main Library Functions */
IntDynamicArray* CreateDynamicIntArray(int _sizeOfArray, int _BlockSizeValueToIncrement)
{
	IntDynamicArray* tempIDAPtr = NULL;
	
	if(CreationCompleteValidationCheckOnParams(_sizeOfArray,_BlockSizeValueToIncrement) != OK)
	{
		return NULL;
	}
	
	tempIDAPtr = malloc(sizeof(IntDynamicArray)); /* returns NULL if error */
	
	if(ValidateFirstInitNotNullIntDynamicArrayPtr(tempIDAPtr) != OK)
	{
		return NULL;
	}
	
	tempIDAPtr->m_DynamicArrayPtr = calloc(_sizeOfArray, sizeof(int)); /* returns NULL if error */
		
	if(ValidateNotNullIntPtr(tempIDAPtr->m_DynamicArrayPtr) != OK)
	{
		free(tempIDAPtr);
		return NULL;
	}
	
	return InitializeIntDynamicArray(tempIDAPtr, _sizeOfArray, _BlockSizeValueToIncrement);
}


void DestroyDynamicIntArray(IntDynamicArray* _intDynamicArray)
{
	if(ValidateNotNullIntDynamicArrayPtr(_intDynamicArray) == OK && _intDynamicArray->m_MagicNumber == MAGIC)
	{
		if(ValidateNotNullIntPtr(_intDynamicArray->m_DynamicArrayPtr) == OK /* Thread saftey: */ && _intDynamicArray->m_MagicNumber == MAGIC)
		{
			free(_intDynamicArray->m_DynamicArrayPtr);
		}
		
		_intDynamicArray->m_MagicNumber = 0; /* A stamp (to mark) when free() was called once already - probability of ~1/2000000000000 to make 2 free()s mistakely that way */
		free(_intDynamicArray);
	}
}


/*
void DestroyDynamicIntArray(IntDynamicArray** _intDynamicArray)
{
	if(ValidateNotNullIntDynamicArrayPtr(*_intDynamicArray) == OK)
	{
		if(ValidateNotNullIntPtr((*_intDynamicArray)->m_DynamicArrayPtr) == OK)
		{
			free((*_intDynamicArray)->m_DynamicArrayPtr);
			(*_intDynamicArray)->m_DynamicArrayPtr = NULL;
		}
	
		free(*_intDynamicArray);
		*_intDynamicArray = NULL;
	}
}
*/

int InsertToIntDynamicArray(IntDynamicArray* _intDynamicArray, int _dataToInsert)
{
	int statusCode;
			
	if((statusCode = InsertionCompleteValidationCheckOnParams(_intDynamicArray)) != OK)
	{
		return statusCode;
	}

	if(_intDynamicArray->m_NumOfElemsInDynamicArray + 1 <= _intDynamicArray->m_DynamicArraySize)
	{
		PushNewElementToDynamicArray(_intDynamicArray, _dataToInsert);
	}
	else
	{
		if((statusCode = TryReallocateMemoryToDynamicArray(_intDynamicArray)) != OK)
		{
			return statusCode;
		}
		/* Else: reallocation completed successfully */
		PushNewElementToDynamicArray(_intDynamicArray, _dataToInsert);
	}
	
	return OK;
}


int RemoveFromIntDynamicArray(IntDynamicArray* _intDynamicArray, int* _removedData)
{
	int statusCode;
	
	if((statusCode = RemoveCompleteValidationCheckOnParams(_intDynamicArray, _removedData)) != OK)
	{
		return statusCode;
	}
	
	PopLastElementFromDynamicArray(_intDynamicArray, _removedData);
	
	return OK;
}


int UpdateBlockSizeValueToIncrement(IntDynamicArray* _intDynamicArray, int _newBlockSizeValue) // MUST APDATE THE HEADER!!!!!!!!!!!
{
	int statusCode;
	
	if((statusCode = ValidateSize(_newBlockSizeValue)) != OK)
	{
		return statusCode;
	}
	
	_intDynamicArray->m_BlockSizeValueToIncrement = _newBlockSizeValue;
	
	return OK;
}


/* Validation Functions: */

int ValidateSize(int _size)
{
	return (_size < 0) ? SIZE_ERROR : OK;
}


int ValidateNotNullIntPtr(int* _intPtr)
{
	return (!_intPtr) ? NULL_PTR_ERROR : OK;
}


int ValidateNotNullIntDynamicArrayPtr(IntDynamicArray* _intDynamicArray)
{
	if(_intDynamicArray)
	{
		return (ValidateNotNullIntPtr(_intDynamicArray->m_DynamicArrayPtr) == NULL_PTR_ERROR) ? NULL_PTR_ERROR : OK;
	}
	else
	{
		return NULL_PTR_ERROR;
	}
}


int ValidateFirstInitNotNullIntDynamicArrayPtr(IntDynamicArray* _intDynamicArray)
{
	return (!_intDynamicArray) ? NULL_PTR_ERROR : OK;
}


int ValidateSuccessfulAllocation(int* _intArrayPtr) /* For better tracing - which error has occured (and it is more readable) */
{
	return (ValidateNotNullIntPtr(_intArrayPtr) == NULL_PTR_ERROR) ? ALLOCATION_FAILED_ERROR : OK;
}


int ValidateNotMemoryOverflowAfterInsert(IntDynamicArray* _intDynamicArray)
{
	return (_intDynamicArray->m_DynamicArraySize + _intDynamicArray->m_BlockSizeValueToIncrement < _intDynamicArray->m_NumOfElemsInDynamicArray + 1) ? MEMORY_OVERFLOW_ERROR : OK;
}


int ValidateNotMemoryUnderflowAfterRemove(IntDynamicArray* _intDynamicArray)
{
	return (_intDynamicArray->m_NumOfElemsInDynamicArray - 1 < 0) ? MEMORY_UNDERFLOW_ERROR : OK;
}


/* Helper Functions: */

int CreationCompleteValidationCheckOnParams(int _sizeOfArray, int _BlockSizeValueToIncrement)
{
	int statusCode;
	
	if((statusCode = ValidateSize(_sizeOfArray)) != OK || (statusCode = ValidateSize(_BlockSizeValueToIncrement)) != OK)
	{
		return statusCode;
	}
	
	if(_sizeOfArray == 0 && _BlockSizeValueToIncrement == 0)
	{
		return SIZE_ERROR;
	}
	
	return OK;
}


IntDynamicArray* InitializeIntDynamicArray(IntDynamicArray* _intDynamicArray, int _sizeOfArray, int _BlockSizeValueToIncrement)
{
	_intDynamicArray->m_DynamicArraySize = _sizeOfArray;
	_intDynamicArray->m_BlockSizeValueToIncrement = _BlockSizeValueToIncrement;
	_intDynamicArray->m_NumOfElemsInDynamicArray = 0;
	_intDynamicArray->m_MagicNumber = MAGIC;

	return _intDynamicArray;
}


int InsertionCompleteValidationCheckOnParams(IntDynamicArray* _intDynamicArray)
{
	int statusCode;

	if((statusCode = ValidateNotNullIntDynamicArrayPtr(_intDynamicArray)) != OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotMemoryOverflowAfterInsert(_intDynamicArray)) != OK)
	{
		return statusCode;
	}	

	return OK;
}


int RemoveCompleteValidationCheckOnParams(IntDynamicArray* _intDynamicArray, int* _removedData)
{
	int statusCode;

	if(((statusCode = ValidateNotNullIntDynamicArrayPtr(_intDynamicArray)) != OK) || ((statusCode = ValidateNotNullIntPtr(_removedData)) != OK))
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotMemoryUnderflowAfterRemove(_intDynamicArray)) != OK)
	{
		return statusCode;
	}
	
	return OK;
}


void PushNewElementToDynamicArray(IntDynamicArray* _intDynamicArray, int _dataToInsert)
{
	_intDynamicArray->m_DynamicArrayPtr[_intDynamicArray->m_NumOfElemsInDynamicArray] = _dataToInsert;
	_intDynamicArray->m_NumOfElemsInDynamicArray++;
}


int TryReallocateMemoryToDynamicArray(IntDynamicArray* _intDynamicArray)
{
	int statusCode;
	int* temp = realloc(_intDynamicArray->m_DynamicArrayPtr, ((_intDynamicArray->m_DynamicArraySize + _intDynamicArray->m_BlockSizeValueToIncrement) * sizeof(int)));
	
	if((statusCode = ValidateSuccessfulAllocation(temp)) != OK)
	{
		return statusCode;
	}
	
	_intDynamicArray->m_DynamicArrayPtr = temp;
	_intDynamicArray->m_DynamicArraySize += _intDynamicArray->m_BlockSizeValueToIncrement;
	
	return OK;
}


void PopLastElementFromDynamicArray(IntDynamicArray* _intDynamicArray, int* _removedData)
{
	_intDynamicArray->m_NumOfElemsInDynamicArray--;
	*_removedData = _intDynamicArray->m_DynamicArrayPtr[_intDynamicArray->m_NumOfElemsInDynamicArray];
	_intDynamicArray->m_DynamicArrayPtr[_intDynamicArray->m_NumOfElemsInDynamicArray] = 0;
}


