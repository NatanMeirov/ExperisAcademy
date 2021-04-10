#include <stdio.h> /* printf */
#include <stddef.h> /* size_t */
#include "../../Include/GenericVector/GenericVector.h"

#define LENGTH 3
#define BLOCK_SIZE 2
#define BIG_LENGTH 100000000
#define BIG_BLOCK_SIZE 25000000
#define TO_BIG_BLOCK_SIZE ((size_t) -2) /* Will be almost the biggest number as size_t (almost all bits are 1) */
#define BYTES_CHUNK 4096
#define TO_BIG_INDEX 100


void Test_VectorCreate(void);
void Test_Reg_VectorCreate(void);
void Test_BigSize_VectorCreate(void);
void Test_ZeroSize_VectorCreate(void);
void Test_ZeroBlockSize_VectorCreate(void);
void Test_ZeroSizeZeroBlockSize_VectorCreate(void);
void Test_VectorDestroy(void);
void Test_Reg_VectorDestroy(void);
void Test_Null_VectorDestroy(void);
void Test_VectorAppend(void);
void Test_Reg_VectorAppend(void);
void Test_Null_VectorAppend(void);
void Test_IncreaseMemory_VectorAppend(void);
void Test_OverFlow_VectorAppend(void);
void Test_BigBlockFailedReAllocation_VectorAppend(void);
void Test_VectorRemove(void);
void Test_Reg_VectorRemove(void);
void Test_UnderFlow_VectorRemove(void);
void Test_Null_VectorRemove(void);
void Test_ShrinkVector_VectorRemove(void);
void Test_VectorGet(void);
void Test_Reg_VectorGet(void);
void Test_Null_VectorGet(void);
void Test_WrongIndex_VectorGet(void);
void Test_VectorSet(void);
void Test_Reg_VectorSet(void);
void Test_Null_VectorSet(void);
void Test_WrongIndex_VectorSet(void);
void Test_VectorSize(void);
void Test_Reg_VectorSize(void);
void Test_Null_VectorSize(void);
void Test_VectorUpdateExtensionBlockSize(void);
void Test_Reg_VectorUpdateExtensionBlockSize(void);
void Test_Null_VectorUpdateExtensionBlockSize(void);


int main(void)
{
	/* Main Tests: */
	Test_VectorCreate();
	Test_VectorDestroy();
	Test_VectorAppend();
	Test_VectorRemove();
	Test_VectorGet();
	/*Test_VectorSet();
	Test_VectorSize();
	Test_VectorUpdateExtensionBlockSize();*/

	return 0;
}


/* Util Functions: */
int VectorElementFindData(void* _elm, size_t _index, void* _context)
{
	if(*((int*)_elm) == *((int*)_context))
	{
		return 0; /* Data Found */
	}

	return 1;
}

int VectorElementFindAddrOfData(void* _elm, size_t _index, void* _context)
{
	if((int*)_elm == (int*)_context)
	{
		return 0; /* Address of Data Found */
	}

	return 1;
}


int VectorElementPrint(void* _elm, size_t _index, void* _context)
{
	printf("%d\n", *((int*)_elm));

	return 1; /* Continue */
}


/* VectorCreate Tests: */
void Test_VectorCreate(void)
{
	Test_Reg_VectorCreate();
	Test_BigSize_VectorCreate();
	Test_ZeroSize_VectorCreate();
	Test_ZeroBlockSize_VectorCreate();
	Test_ZeroSizeZeroBlockSize_VectorCreate();
}

void Test_Reg_VectorCreate(void)
{
	Vector* vec = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_Reg_VectorCreate: Failed...\n");
		return;
	}

	if(VectorCapacity(vec) == LENGTH && VectorSize(vec) == 0)
	{
		printf("Test_Reg_VectorCreate: Passed!\n");
	}

	VectorDestroy(&vec, NULL);
}

void Test_BigSize_VectorCreate(void)
{
	Vector* vec = NULL;

	vec = VectorCreate(BIG_LENGTH, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_BigSize_VectorCreate: Failed...\n");
		return;
	}

	if(VectorCapacity(vec) == BIG_LENGTH && VectorSize(vec) == 0)
	{
		printf("Test_BigSize_VectorCreate: Passed!\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_ZeroSize_VectorCreate()
{
	Vector* vec = NULL;

	vec = VectorCreate(0, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_ZeroSize_VectorCreate: Failed...\n");
	}

	if(VectorCapacity(vec) == 0 && VectorSize(vec) == 0)
	{
		printf("Test_ZeroSize_VectorCreate: Passed!\n");
		VectorDestroy(&vec, NULL);
	}
	else
	{
		printf("Test_ZeroSize_VectorCreate: Failed...\n");
	}
}


void Test_ZeroBlockSize_VectorCreate()
{
	Vector* vec = NULL;

	vec = VectorCreate(LENGTH, 0);
	if(!vec)
	{
		printf("Test_ZeroBlockSize_VectorCreate: Failed...\n");
	}

	if(VectorCapacity(vec) == LENGTH && VectorSize(vec) == 0)
	{
		printf("Test_ZeroBlockSize_VectorCreate: Passed!\n");
		VectorDestroy(&vec, NULL);
	}
	else
	{
		printf("Test_ZeroBlockSize_VectorCreate: Failed...\n");
	}
}

void Test_ZeroSizeZeroBlockSize_VectorCreate()
{
	Vector* vec = NULL;

	vec = VectorCreate(0, 0);
	if(!vec)
	{
		printf("Test_ZeroSizeZeroBlockSize_VectorCreate: Passed!\n");
	}
	else
	{
		printf("Test_ZeroSizeZeroBlockSize_VectorCreate: Failed...\n");
		VectorDestroy(&vec, NULL);
	}
}


/* VectorDestroy Tests: */

void Test_VectorDestroy(void)
{
	Test_Reg_VectorDestroy();
	Test_Null_VectorDestroy();
}



void Test_Null_VectorDestroy(void)
{
	VectorDestroy(NULL, NULL); /* Check if not segmentation fault */

	printf("Test_Null_VectorDestroy: Passed!\n");
}


void Test_Reg_VectorDestroy(void)
{
	Vector* vec = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	VectorDestroy(&vec, NULL);
	if(!vec)
	{
		printf("Test_DoubleDestroy_VectorDestroy: Passed!\n");
	}
	else
	{
		printf("Test_DoubleDestroy_VectorDestroy: Failed...\n");
	}
}


/* VectorAppend Tests: */

void Test_VectorAppend(void)
{
	Test_Reg_VectorAppend();
	Test_Null_VectorAppend();
	Test_IncreaseMemory_VectorAppend();
	Test_OverFlow_VectorAppend();
	Test_BigBlockFailedReAllocation_VectorAppend();
}


void Test_Reg_VectorAppend(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int data = 10;
	int anotherData = 5;
	int dataTest = 10;
	int anotherDataTest = 5;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	statusCode = VectorAppend(vec, &data);

	if(statusCode == VECTOR_SUCCESS && VectorSize(vec) == 1 && VectorForEach(vec, &VectorElementFindData, &dataTest) == 0 /*index*/)
	{
		statusCode = VectorAppend(vec, &anotherData);

		if(statusCode == VECTOR_SUCCESS && VectorSize(vec) == 2 && VectorForEach(vec, &VectorElementFindData, &anotherDataTest) == 1 /*index*/)
		{
			printf("Test_Reg_VectorAppend: Passed!\n");
			printf("Vector:\n");
			VectorForEach(vec, &VectorElementPrint, NULL);
		}
		else
		{
			printf("Test_Reg_VectorAppend: [In second insertion:] Failed...\n");
		}
	}
	else
	{
		printf("Test_Reg_VectorAppend: [In first insertion:] Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_Null_VectorAppend(void)
{
	VectorResult statusCode;
	int data = 10;

	if((statusCode = VectorAppend(NULL, &data) == VECTOR_UNINITIALIZED_ERROR))
	{
		printf("Test_Null_VectorAppend: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorAppend: Failed...\n");
	}
}


void Test_IncreaseMemory_VectorAppend(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int data = 10;
	int anotherData = 5;

	vec = VectorCreate(1, BLOCK_SIZE);

	VectorAppend(vec, &data);

	statusCode = VectorAppend(vec, &anotherData); /* Should increase (reallocating) memory */


	if(statusCode == VECTOR_SUCCESS && VectorSize(vec) == 2 && VectorForEach(vec, &VectorElementFindAddrOfData, &anotherData) == 1 /*index*/ && VectorCapacity(vec) == 1 + BLOCK_SIZE)
	{
		printf("Test_IncreaseMemory_VectorAppend: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_IncreaseMemory_VectorAppend: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}

void Test_OverFlow_VectorAppend(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int data = 10;
	int anotherData = 5;

	vec = VectorCreate(1, 0);

	VectorAppend(vec, &data);

	statusCode = VectorAppend(vec, &anotherData); /* Should fail (Over Flow) */

	if(statusCode == VECTOR_OVERFLOW_ERROR && VectorSize(vec) == 1 && VectorForEach(vec, &VectorElementFindAddrOfData, &data) == 0 /*did not changed from index 0*/ && VectorCapacity(vec) == 1)
	{
		printf("Test_OverFlow_VectorAppend: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_OverFlow_VectorAppend: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}

void Test_BigBlockFailedReAllocation_VectorAppend(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int data = 10;
	int anotherData = 5;

	vec = VectorCreate(1, TO_BIG_BLOCK_SIZE);

	VectorAppend(vec, &data);

	statusCode = VectorAppend(vec, &anotherData);

	if(statusCode == VECTOR_ALLOCATION_ERROR && VectorSize(vec) == 1 && VectorForEach(vec, &VectorElementFindAddrOfData, &data) == 0 /*did not changed from index 0*/ && VectorCapacity(vec) == 1)
	{
		printf("Test_BigBlockFailedReAllocation_VectorAppend: Passed!\n");
	}
	else
	{
		printf("Test_BigBlockFailedReAllocation_VectorAppend: Failed... (Optional: Successful ReAllocation of %ld Bytes - Passed Reg Insertion)\n", TO_BIG_BLOCK_SIZE);
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}

	VectorDestroy(&vec, NULL);
}


/* VectorRemove Test: */

void Test_VectorRemove(void)
{
	Test_Reg_VectorRemove();
	Test_UnderFlow_VectorRemove();
	Test_Null_VectorRemove();
	Test_ShrinkVector_VectorRemove();
}


void Test_Reg_VectorRemove(void)
{
	VectorResult statusCode;
	int* tempNum = NULL;
	int data = 10;
	int anotherData = 5;
	Vector* vec = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	VectorAppend(vec, &data);
	VectorAppend(vec, &anotherData);

	if((statusCode = VectorRemove(vec, (void**)&tempNum) == VECTOR_SUCCESS) && tempNum == &anotherData && *tempNum == 5 && VectorSize(vec) == 1)
	{
		if((statusCode = VectorRemove(vec, (void**)&tempNum) == VECTOR_SUCCESS) && tempNum == &data && *tempNum == 10 && VectorSize(vec) == 0)
		{
			printf("Test_Reg_VectorRemove: Passed!\n");
			printf("Vector:\n");
			VectorForEach(vec, &VectorElementPrint, NULL);
		}
		else
		{
			printf("Test_Reg_VectorRemove: [In the second remove:] Failed...\n");
		}
	}
	else
	{
		printf("Test_Reg_VectorRemove: [In the first remove:] Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_UnderFlow_VectorRemove(void)
{
	VectorResult statusCode;
	int* tempNum = NULL;
	int data = 10;
	Vector* vec = NULL;

	vec = VectorCreate(1, BLOCK_SIZE);

	VectorAppend(vec, &data);

	VectorRemove(vec, (void**)&tempNum);


	if((statusCode = VectorRemove(vec, (void**)&tempNum)) == VECTOR_UNDERFLOW_ERROR && VectorSize(vec) == 0)
	{
		printf("Test_UnderFlow_VectorRemove: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_UnderFlow_VectorRemove: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}

void Test_Null_VectorRemove(void)
{
	VectorResult statusCode;
	int* tempNum = NULL;
	Vector* vec = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	if((statusCode = VectorRemove(NULL, (void**)&tempNum)) == VECTOR_UNINITIALIZED_ERROR && tempNum == NULL && (statusCode = VectorRemove(vec, NULL)) == VECTOR_UNINITIALIZED_ERROR && (statusCode = VectorRemove(NULL, NULL)) == VECTOR_UNINITIALIZED_ERROR)
	{
		printf("Test_Null_VectorRemove: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorRemove: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_ShrinkVector_VectorRemove(void)
{
	int i, hasChangedSizeOnDemand = 0, hasStoppedAtOriginalSize = 0;
	int* tempNum = NULL;
	size_t currentSize = 0;
	Vector* vec = NULL;
	VectorResult statusCode;
	int data = 5;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	for(i = 0; i < LENGTH + (BLOCK_SIZE * 10) + 1; i++)
	{
		statusCode = VectorAppend(vec, &data);
	}

	currentSize = VectorCapacity(vec);

	for(i = 0; i < LENGTH + (BLOCK_SIZE * 10) + 1; i++)
	{
		statusCode = VectorRemove(vec, (void**)&tempNum);

		if(currentSize > VectorCapacity(vec))
		{
			hasChangedSizeOnDemand = 1;
		}
	}

	if(VectorCapacity(vec) == LENGTH)
	{
		hasStoppedAtOriginalSize = 1;
	}

	if(hasChangedSizeOnDemand && hasStoppedAtOriginalSize && statusCode == VECTOR_SUCCESS && VectorSize(vec) == 0 && tempNum == & data && *tempNum == 5)
	{
		printf("Test_ShrinkVector_VectorRemove: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_ShrinkVector_VectorRemove: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


/* VectorGet Tests: */

void Test_VectorGet(void)
{
	Test_Reg_VectorGet();
	Test_Null_VectorGet();
	Test_WrongIndex_VectorGet();
}


void Test_Reg_VectorGet(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int* tempNum = NULL;
	int* anotherTempNum = NULL;
	int data = 10, anotherData = 5;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	VectorAppend(vec, &data);
	VectorAppend(vec, &anotherData);

	if((statusCode = VectorGet(vec, 0, (void**)&tempNum)) == VECTOR_SUCCESS && *tempNum == data && (statusCode = VectorGet(vec, 1, (void**)&anotherTempNum)) == VECTOR_SUCCESS && *anotherTempNum == anotherData && VectorSize(vec) == 2 /* Make sure not deleting items */ && *anotherTempNum != *tempNum)
	{
		printf("Test_Reg_VectorGet: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_Reg_VectorGet: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_Null_VectorGet(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int* tempNum = NULL;
	int data = 10, anotherData = 5;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	VectorAppend(vec, &data);
	VectorAppend(vec, &anotherData);

	if((statusCode = VectorGet(NULL, 0, (void**)&tempNum)) == VECTOR_UNINITIALIZED_ERROR && tempNum == NULL && (statusCode = VectorGet(vec, 1, NULL)) == VECTOR_UNINITIALIZED_ERROR && (statusCode = VectorGet(NULL, 0, NULL)) == VECTOR_UNINITIALIZED_ERROR)
	{
		printf("Test_Null_VectorGet: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_Null_VectorGet: Failed...\n");
	}

	VectorDestroy(&vec, NULL);
}


void Test_WrongIndex_VectorGet(void)
{
	VectorResult statusCode;
	Vector* vec = NULL;
	int* tempNum = NULL;
	int* anotherTempNum = NULL;
	int data = 10, anotherData = 5;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);

	VectorAppend(vec, &data);
	VectorAppend(vec, &anotherData);
	VectorAppend(vec, &data);

	if((statusCode = VectorGet(vec, TO_BIG_INDEX, (void**)&tempNum)) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR && tempNum == NULL && (statusCode = VectorGet(vec, LENGTH, (void**)&anotherTempNum)) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR && anotherTempNum == NULL)
	{
		printf("Test_WrongIndex_VectorGet: Passed!\n");
		printf("Vector:\n");
		VectorForEach(vec, &VectorElementPrint, NULL);
	}
	else
	{
		printf("Test_WrongIndex_VectorGet: Failed...\n");

	}

	VectorDestroy(&vec, NULL);
}


/* VectorSet Tests: */
/*
void Test_VectorSet(void)
{
	Test_Reg_VectorSet();
	Test_Null_VectorSet();
	Test_WrongIndex_VectorSet();
}


void Test_Reg_VectorSet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, OTHER_DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(vec, 0, OTHER_DATA_NUM)) == VECTOR_SUCCESS && itemsArray[0] == OTHER_DATA_NUM && (statusCode = VectorSet(vec, 1, DATA_NUM)) == VECTOR_SUCCESS && itemsArray[1] == DATA_NUM && VectorSize(vec) == 2) // Make sure not deleting items
	{
		printf("Test_Reg_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_Reg_VectorSet: Failed...\n");
		
	}
	
	VectorDestroy(&vec, NULL);
}


void Test_Null_VectorSet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(NULL, 0, OTHER_DATA_NUM)) == VECTOR_UNINITIALIZED_ERROR && itemsArray[0] != OTHER_DATA_NUM)
	{
		printf("Test_Null_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorSet: Failed...\n");
		
	}
	
	VectorDestroy(&vec, NULL);
}


void Test_WrongIndex_VectorSet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(vec, TO_BIG_INDEX, OTHER_DATA_NUM)) == ERR_WRONG_INDEX && (statusCode = VectorSet(vec, LENGTH, OTHER_DATA_NUM)) == ERR_WRONG_INDEX && itemsArray[VectorSize(vec) - 1] != OTHER_DATA_NUM)
	{
		printf("Test_WrongIndex_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_WrongIndex_VectorSet: Failed...\n");
		
	}
	
	VectorDestroy(&vec, NULL);
}
*/

/* VectorSize Tests: */
/*
void Test_VectorSize(void)
{
	Test_Reg_VectorSize();
	Test_Null_VectorSize();
}


void Test_Reg_VectorSize(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t itemsNum = 0;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, OTHER_DATA_NUM);
	
	if((statusCode = VectorSize(vec, &itemsNum)) == VECTOR_SUCCESS && itemsNum == 4)
	{
		printf("Test_Reg_VectorSize: Passed!\n");
	}
	else
	{
		printf("Test_Reg_VectorSize: Failed...\n");
		
	}
	
	VectorDestroy(&vec, NULL);
}


void Test_Null_VectorSize(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t itemsNum = 0;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAppend(vec, DATA_NUM);
	VectorAppend(vec, DATA_NUM);
	
	if((statusCode = VectorSize(vec, NULL)) == VECTOR_UNINITIALIZED_ERROR && (statusCode = VectorSize(NULL, &itemsNum)) == VECTOR_UNINITIALIZED_ERROR && itemsNum == 0 && (statusCode = VectorSize(NULL, NULL) == VECTOR_UNINITIALIZED_ERROR))
	{
		printf("Test_Null_VectorSize: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorSize: Failed...\n");
		
	}
	
	VectorDestroy(&vec, NULL);
}
*/
