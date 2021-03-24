/* Libraries: */
#include <stdlib.h> /* Using malloc/calloc from stdlib.h */
#include "Dynamic_Allocation_Int_Array_Management_Library.h"


/* Validation Functions Declarations: */
int ValidateSize(int _size);
int ValidateNotNullPtr(int* _dynamicArrayPtr);
int ValidateNotNullPtrToPtr(int** _ptrToDynamicArrayPtr);
int ValidateSuccessfulAllocation(int* _dynamicArrayPtr);
int ValidateNotMemoryOverflowAfterInsert(int _numOfElemsInDynamicArray, int _sizeOfDynamicArray, int _blockSizeValueToIncrement);
int ValidateNotMemoryUnderflowAfterRemove(int _numOfElemsInDynamicArray);

/* Helper Functions Declarations: */
int InsertionCompleteValidationCheckOnParams(int** _dynamicArrayPtr, int* _dynamicArraySize, int* _numOfElemsInDynamicArray, int _blockSizeValueToIncrement);
int RemoveCompleteValidationCheckOnParams(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray);
void PushNewElementToDynamicArray(int *_dynamicArrayPtr, int *_numOfElemsInDynamicArray, int _dataToInsert);
int TryReallocateMemoryToDynamicArray(int** _dynamicArrayPtr, int* _sizeOfDynamicArray, int _blockSizeValueToIncrement);
void PopLastElementFromDynamicArray(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray);


/* Main Library Functions */
int* CreateDynamicIntArray(int _sizeOfArray)
{
	/* int* dynamicArrayPtr = NULL; */
	int statusCode;
	
	if(statusCode = ValidateSize(_sizeOfArray) != OK)
	{
		return NULL;
	}
	
	if(_sizeOfArray == 0)
	{
		return NULL;
	}
	
	return calloc(_sizeOfArray, sizeof(int)); /* returns NULL if error */
	
	/* dynamicArrayPtr = malloc(_sizeOfArray * sizeof(int)); */
	/* dynamicArrayPtr = calloc(_sizeOfArray, sizeof(int)); */
	/* return (ValidateSuccessfulAllocation(dynamicArrayPtr) != ALLOCATION_FAILED_ERROR) ? dynamicArrayPtr : NULL; */
}


void DestroyDynamicIntArray(int* _dynamicArrayPtr)
{
	if(ValidateNotNullPtr(_dynamicArrayPtr) == OK)
	{
		free(_dynamicArrayPtr);
	}
}


int InsertToIntDynamicArray(int** _dynamicArrayPtr, int _dataToInsert, int* _dynamicArraySize, int* _numOfElemsInDynamicArray, int _blockSizeValueToIncrement)
{
	int statusCode;
	
	_blockSizeValueToIncrement = (_blockSizeValueToIncrement < 0) ? 0 : _blockSizeValueToIncrement;
		
	if((statusCode = InsertionCompleteValidationCheckOnParams(_dynamicArrayPtr, _dynamicArraySize, _numOfElemsInDynamicArray, _blockSizeValueToIncrement)) != OK)
	{
		return statusCode;
	}


	if(*_numOfElemsInDynamicArray + 1 <= *_dynamicArraySize)
	{
		PushNewElementToDynamicArray(*_dynamicArrayPtr, _numOfElemsInDynamicArray, _dataToInsert);
	}
	else
	{
		if((statusCode = TryReallocateMemoryToDynamicArray(_dynamicArrayPtr, _dynamicArraySize, _blockSizeValueToIncrement)) != OK)
		{
			return statusCode;
		}
		/* Else: reallocation completed successfully */
		PushNewElementToDynamicArray(*_dynamicArrayPtr, _numOfElemsInDynamicArray, _dataToInsert);
	}
	
	return OK;
}


int RemoveFromIntDynamicArray(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray)
{
	int statusCode;
	
	if((statusCode = RemoveCompleteValidationCheckOnParams(_dynamicArrayPtr, _removedData, _numOfElemsInDynamicArray)) != OK)
	{
		return statusCode;
	}
	
	PopLastElementFromDynamicArray(_dynamicArrayPtr, _removedData, _numOfElemsInDynamicArray);
	
	return OK;
}


/* Validation Functions: */

int ValidateSize(int _size)
{
	return (_size < 0) ? SIZE_ERROR : OK;
}


int ValidateNotNullPtr(int* _dynamicArrayPtr)
{
	return (!_dynamicArrayPtr) ? NULL_PTR_ERROR : OK;
}

int ValidateNotNullPtrToPtr(int** _ptrToDynamicArrayPtr)
{
	return (!_ptrToDynamicArrayPtr) ? NULL_PTR_ERROR : OK;
}


int ValidateSuccessfulAllocation(int* _dynamicArrayPtr) /* For better tracing - which error has occured (and it is more readable) */
{
	return (ValidateNotNullPtr(_dynamicArrayPtr) == NULL_PTR_ERROR) ? ALLOCATION_FAILED_ERROR : OK;
}


int ValidateNotMemoryOverflowAfterInsert(int _numOfElemsInDynamicArray, int _sizeOfDynamicArray, int _blockSizeValueToIncrement)
{
	return (_sizeOfDynamicArray + _blockSizeValueToIncrement < _numOfElemsInDynamicArray + 1) ? MEMORY_OVERFLOW_ERROR : OK;
}


int ValidateNotMemoryUnderflowAfterRemove(int _numOfElemsInDynamicArray)
{
	return (_numOfElemsInDynamicArray - 1 < 0) ? MEMORY_UNDERFLOW_ERROR : OK;
}


/* Helper Functions: */

int InsertionCompleteValidationCheckOnParams(int** _dynamicArrayPtr, int* _dynamicArraySize, int* _numOfElemsInDynamicArray, int _blockSizeValueToIncrement)
{
	int statusCode;

	if(((statusCode = ValidateNotNullPtr(_dynamicArraySize)) != OK) || ((statusCode = ValidateNotNullPtr(_numOfElemsInDynamicArray)) != OK) || ((statusCode = ValidateNotNullPtrToPtr(_dynamicArrayPtr)) != OK) || ((statusCode = ValidateNotNullPtr(*_dynamicArrayPtr)) != OK))
	{
		return statusCode;
	}

	if((statusCode = ValidateSize(*_dynamicArraySize)) != OK)
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotMemoryOverflowAfterInsert(*_numOfElemsInDynamicArray, *_dynamicArraySize, _blockSizeValueToIncrement)) != OK)
	{
		return statusCode;
	}	

	return OK;
}


int RemoveCompleteValidationCheckOnParams(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray)
{
	int statusCode;

	if(((statusCode = ValidateNotNullPtr(_dynamicArrayPtr)) != OK) || ((statusCode = ValidateNotNullPtr(_removedData)) != OK) || ((statusCode = ValidateNotNullPtr(_numOfElemsInDynamicArray)) != OK))
	{
		return statusCode;
	}
	
	if((statusCode = ValidateNotMemoryUnderflowAfterRemove(*_numOfElemsInDynamicArray)) != OK)
	{
		return statusCode;
	}
	
	return OK;
}


void PushNewElementToDynamicArray(int* _dynamicArrayPtr, int* _numOfElemsInDynamicArray, int _dataToInsert)
{
	_dynamicArrayPtr[*_numOfElemsInDynamicArray] = _dataToInsert;
	(*_numOfElemsInDynamicArray)++;
}


int TryReallocateMemoryToDynamicArray(int** _dynamicArrayPtr, int* _sizeOfDynamicArray, int _blockSizeValueToIncrement)
{
	int statusCode;
	int* temp = realloc(*_dynamicArrayPtr, ((*_sizeOfDynamicArray + _blockSizeValueToIncrement) * sizeof(int)));
	
	if((statusCode = ValidateSuccessfulAllocation(temp)) != OK)
	{
		return statusCode;
	}
	
	*_dynamicArrayPtr = temp;
	(*_sizeOfDynamicArray) += _blockSizeValueToIncrement;
	
	return OK;
}


void PopLastElementFromDynamicArray(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray)
{
	(*_numOfElemsInDynamicArray)--;
	*_removedData = _dynamicArrayPtr[*_numOfElemsInDynamicArray];
	_dynamicArrayPtr[*_numOfElemsInDynamicArray] = 0;
}


