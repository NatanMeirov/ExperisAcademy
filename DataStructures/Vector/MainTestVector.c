#include <stdio.h>
#include "ADTDefs.h"
#include "Vector.h"
#include "TestVector.h"

#define LENGTH 3
#define BLOCK_SIZE 2
#define BIG_LENGTH 100000000
#define BIG_BLOCK_SIZE 25000000
#define TO_BIG_BLOCK_SIZE -2 /* Will be almost the biggest number as size_t (almost all bits are 1) */
#define DATA_NUM 5
#define OTHER_DATA_NUM 10
#define BYTES_CHUNK 4096
#define TO_BIG_INDEX 100
#define NEW_MAGIC_NUM 5198322


void PrintError(ADTErr _error);
void Test_VectorCreate(void);
void Test_Reg_VectorCreate(void);
void Test_BigSize_VectorCreate(void);
void Test_ZeroSize_VectorCreate(void);
void Test_ZeroBlockSize_VectorCreate(void);
void Test_ZeroSizeZeroBlockSize_VectorCreate(void);
void Test_VectorDestroy(void);
void Test_Reg_VectorDestroy(void);
void Test_Null_VectorDestroy(void);
void Test_DoubleDestroy_VectorDestroy(void);
void Test_OtherMagicNumber_VectorDestroy(void);
void Test_VectorAdd(void);
void Test_Reg_VectorAdd(void);
void Test_Null_VectorAdd(void);
void Test_IncreaseMemory_VectorAdd(void);
void Test_OverFlow_VectorAdd(void);
void Test_BigBlockFailedReAllocation_VectorAdd(void);
void Test_ChangedPtrAddressAfterReAllocation_VectorAdd(void);
void Test_VectorDelete(void);
void Test_Reg_VectorDelete(void);
void Test_UnderFlow_VectorDelete(void);
void Test_Null_VectorDelete(void);
void Test_ShrinkVector_VectorDelete(void);
void Test_VectorGet(void);
void Test_Reg_VectorGet(void);
void Test_Null_VectorGet(void);
void Test_WrongIndex_VectorGet(void);
void Test_VectorSet(void);
void Test_Reg_VectorSet(void);
void Test_Null_VectorSet(void);
void Test_WrongIndex_VectorSet(void);
void Test_VectorGetItemsNumber(void);
void Test_Reg_VectorGetItemsNumber(void);
void Test_Null_VectorGetItemsNumber(void);
void Test_VectorUpdateExtensionBlockSize(void);
void Test_Reg_VectorUpdateExtensionBlockSize(void);
void Test_Null_VectorUpdateExtensionBlockSize(void);
void GeneralTest_PrintVector(void);


int main(void)
{
	/* Main Tests: */
	Test_VectorCreate();
	Test_VectorDestroy();
	Test_VectorAdd();
	Test_VectorDelete();
	Test_VectorSet();
	Test_VectorGetItemsNumber();
	Test_VectorUpdateExtensionBlockSize();
	GeneralTest_PrintVector();
		
	return 0;
}


void PrintError(ADTErr _error)
{
	printf("%s\n", HandleError(_error));
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
	int* itemsArray = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_Reg_VectorCreate: Failed...\n");
		return;
	}
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(itemsArray && GetSizeOfVector(vec) == LENGTH && GetVectorExtensionBlockSize(vec) == BLOCK_SIZE && GetNumberOfItemsInVector(vec) == 0 && GetOriginalVectorSize(vec) == LENGTH)
	{
		printf("Test_Reg_VectorCreate: Passed!\n");
	}
	
	VectorDestroy(vec);
}

void Test_BigSize_VectorCreate(void)
{
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(BIG_LENGTH, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_BigSize_VectorCreate: Failed...\n");
		return;
	}
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(itemsArray && GetSizeOfVector(vec) == BIG_LENGTH && GetVectorExtensionBlockSize(vec) == BLOCK_SIZE && GetNumberOfItemsInVector(vec) == 0 && GetOriginalVectorSize(vec) == BIG_LENGTH)
	{
		printf("Test_BigSize_VectorCreate: Passed!\n");
	}
	
	VectorDestroy(vec);
}


void Test_ZeroSize_VectorCreate()
{
	Vector* vec = NULL;
	int* itemsArray = NULL;	
		
	vec = VectorCreate(0, BLOCK_SIZE);
	if(!vec)
	{
		printf("Test_ZeroSize_VectorCreate: Failed...\n");
	}
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(itemsArray && GetSizeOfVector(vec) == 0 && GetVectorExtensionBlockSize(vec) == BLOCK_SIZE && GetNumberOfItemsInVector(vec) == 0 && GetOriginalVectorSize(vec) == 0)
	{
		printf("Test_ZeroSize_VectorCreate: Passed!\n");
		VectorDestroy(vec);
	}
	else
	{
		printf("Test_ZeroSize_VectorCreate: Failed...\n");
	}
}


void Test_ZeroBlockSize_VectorCreate()
{
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(LENGTH, 0);
	if(!vec)
	{
		printf("Test_ZeroBlockSize_VectorCreate: Failed...\n");
	}
	
	itemsArray = GetVectorItemsArray(vec); 
	
	if(itemsArray && GetSizeOfVector(vec) == LENGTH && GetVectorExtensionBlockSize(vec) == 0 && GetNumberOfItemsInVector(vec) == 0 && GetOriginalVectorSize(vec) == LENGTH)
	{
		printf("Test_ZeroBlockSize_VectorCreate: Passed!\n");
		VectorDestroy(vec);
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
		VectorDestroy(vec);
	}
}


/* VectorDestroy Tests: */
void Test_VectorDestroy(void)
{
	Test_Reg_VectorDestroy();
	Test_Null_VectorDestroy();
	Test_DoubleDestroy_VectorDestroy();
	Test_OtherMagicNumber_VectorDestroy();
}


void Test_OtherMagicNumber_VectorDestroy(void)
{
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	SetVectorMagicNumber(vec, NEW_MAGIC_NUM);	
	
	VectorDestroy(vec); /* Should not destroy the vector */
	
	itemsArray = GetVectorItemsArray(vec); 
	
	if(itemsArray != NULL /* If not destroyed */)
	{
		printf("Test_OtherMagicNumber_VectorDestroy: Passed!\n");
	}	
	else
	{
		printf("Test_OtherMagicNumber_VectorDestroy: Failed...\n");
	}
}


void Test_DoubleDestroy_VectorDestroy(void)
{
	Vector* vec = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorDestroy(vec);
	VectorDestroy(vec); /* Should not work - else probably segmentation fault */
	
	printf("Test_DoubleDestroy_VectorDestroy: Passed!\n");	
}


void Test_Null_VectorDestroy(void)
{
	VectorDestroy(NULL);
	
	printf("Test_Null_VectorDestroy: Passed!\n");	
}


void Test_Reg_VectorDestroy(void)
{
	Vector* vec = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorDestroy(vec);
		
	printf("Test_Reg_VectorDestroy: Passed!\n");	
}


/* VectorAdd Tests: */
void Test_VectorAdd(void)
{
	Test_Reg_VectorAdd();
	Test_Null_VectorAdd();
	Test_IncreaseMemory_VectorAdd();
	Test_OverFlow_VectorAdd();
	Test_BigBlockFailedReAllocation_VectorAdd();
	Test_ChangedPtrAddressAfterReAllocation_VectorAdd();
}


void Test_Reg_VectorAdd(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	statusCode = VectorAdd(vec, DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(statusCode == ERR_OK && GetNumberOfItemsInVector(vec) == 1 && itemsArray[GetNumberOfItemsInVector(vec) - 1] == DATA_NUM)
	{
		statusCode = VectorAdd(vec, OTHER_DATA_NUM);
		
		if(statusCode == ERR_OK && GetNumberOfItemsInVector(vec) == 2 && itemsArray[GetNumberOfItemsInVector(vec) - 1] == OTHER_DATA_NUM)
		{
			printf("Test_Reg_VectorAdd: Passed!\n");
		}
		else
		{
			printf("Test_Reg_VectorAdd: [In second insertion:] Failed...\n");
			printf("-------> Error: ");
			PrintError(statusCode);
		}
	}
	else
	{
		printf("Test_Reg_VectorAdd: [In first insertion:] Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_Null_VectorAdd(void)
{
	ADTErr statusCode;
	
	if((statusCode = VectorAdd(NULL, DATA_NUM) == ERR_NOT_INITIALIZED))
	{
		printf("Test_Null_VectorAdd: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorAdd: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
}


void Test_IncreaseMemory_VectorAdd(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(1, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	
	statusCode = VectorAdd(vec, OTHER_DATA_NUM); /* Should increase (reallocating) memory */
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(statusCode == ERR_OK && GetNumberOfItemsInVector(vec) == 2 && itemsArray[GetNumberOfItemsInVector(vec) - 1] == OTHER_DATA_NUM && GetSizeOfVector(vec) == 1 + BLOCK_SIZE)
	{
		printf("Test_IncreaseMemory_VectorAdd: Passed!\n");
	}
	else
	{
		printf("Test_IncreaseMemory_VectorAdd: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}

void Test_OverFlow_VectorAdd(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(1, 0);

	VectorAdd(vec, DATA_NUM);	

	statusCode = VectorAdd(vec, OTHER_DATA_NUM); /* Should fail (Over Flow) */
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(statusCode == ERR_OVERFLOW && GetNumberOfItemsInVector(vec) == 1 && itemsArray[GetNumberOfItemsInVector(vec) - 1] != OTHER_DATA_NUM && GetSizeOfVector(vec) == 1)
	{
		printf("Test_OverFlow_VectorAdd: Passed!\n");
	}
	else
	{
		printf("Test_OverFlow_VectorAdd: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}

void Test_BigBlockFailedReAllocation_VectorAdd(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(1, TO_BIG_BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	
	statusCode = VectorAdd(vec, OTHER_DATA_NUM);	
	
	itemsArray = GetVectorItemsArray(vec);
	
	if(statusCode == ERR_REALLOCATION_FAILED && GetNumberOfItemsInVector(vec) == 1 && itemsArray[GetNumberOfItemsInVector(vec) - 1] != OTHER_DATA_NUM && GetSizeOfVector(vec) == 1)
	{
		printf("Test_BigBlockFailedReAllocation_VectorAdd: Passed!\n");
	}
	else
	{
		printf("Test_BigBlockFailedReAllocation_VectorAdd: Failed... (Optional: Successful ReAllocation of %ld Bytes - Passed Reg Insertion)\n", (size_t)TO_BIG_BLOCK_SIZE);
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_ChangedPtrAddressAfterReAllocation_VectorAdd(void)
{
	int* currentAddress = NULL;
	int* itemsArray = NULL;
	char* temp = NULL;
	ADTErr statusCode;
	Vector* vec = NULL;
	
	vec = VectorCreate(1, BIG_BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	
	/* Trying to get new address: */
	temp = malloc(BYTES_CHUNK);
	currentAddress = GetVectorItemsArray(vec);
	
	statusCode = VectorAdd(vec, OTHER_DATA_NUM);	

	itemsArray = GetVectorItemsArray(vec);

	if(statusCode == ERR_OK)
	{
		if(currentAddress != itemsArray)
		{
			printf("Test_ChangedPtrAddressAfterReAllocation_VectorAdd: Passed!\n");
		}
		else
		{
			printf("Test_BigBlockFailedReAllocation_VectorAdd: Failed... (Optional: Successful ReAllocation of %d Bytes using the old address - Passed Reg Insertion)\n", BIG_BLOCK_SIZE);
			printf("-------> Error: ");
			PrintError(statusCode);
		}
	}
	else
	{
		printf("Test_BigBlockFailedReAllocation_VectorAdd: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


/* VectorDelete Test: */
void Test_VectorDelete(void)
{
	Test_Reg_VectorDelete();
	Test_UnderFlow_VectorDelete();
	Test_Null_VectorDelete();
	Test_ShrinkVector_VectorDelete();
}


void Test_Reg_VectorDelete(void)
{
	ADTErr statusCode; 
	int tempNum;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorDelete(vec, &tempNum) == ERR_OK) && tempNum == OTHER_DATA_NUM && GetNumberOfItemsInVector(vec) == 1 && itemsArray[GetNumberOfItemsInVector(vec) - 1] != OTHER_DATA_NUM)
	{
		if((statusCode = VectorDelete(vec, &tempNum) == ERR_OK) && tempNum == DATA_NUM && GetNumberOfItemsInVector(vec) == 0 && itemsArray[0] != DATA_NUM)
		{
			printf("Test_Reg_VectorDelete: Passed!\n");	
		}
		else
		{
			printf("Test_Reg_VectorDelete: [In the second remove:] Failed...\n");
			printf("-------> Error: ");
			PrintError(statusCode);
		}
	}
	else
	{
		printf("Test_Reg_VectorDelete: [In the first remove:] Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_UnderFlow_VectorDelete(void)
{
	ADTErr statusCode;
	int tempNum;
	Vector* vec = NULL;
	int* itemsArray = NULL;
	
	vec = VectorCreate(1, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	
	VectorDelete(vec, &tempNum);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorDelete(vec, &tempNum)) == ERR_UNDERFLOW && GetNumberOfItemsInVector(vec) == 0 && itemsArray[GetNumberOfItemsInVector(vec)] != DATA_NUM)
	{
		printf("Test_UnderFlow_VectorDelete: Passed!\n");
	}
	else
	{
		printf("Test_UnderFlow_VectorDelete: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}

void Test_Null_VectorDelete(void)
{
	ADTErr statusCode;
	int tempNum = -1;
	Vector* vec = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	if((statusCode = VectorDelete(NULL, &tempNum)) == ERR_NOT_INITIALIZED && tempNum == -1 && (statusCode = VectorDelete(vec, NULL)) == ERR_NOT_INITIALIZED && (statusCode = VectorDelete(NULL, NULL)) == ERR_NOT_INITIALIZED)
	{
		printf("Test_Null_VectorDelete: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorDelete: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_ShrinkVector_VectorDelete(void)
{
	int i, tempNum, currentSize, hasChangedSizeOnDemand = 0, hasStoppedAtOriginalSize = 0;
	Vector* vec = NULL;
	ADTErr statusCode;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	for(i = 0; i < LENGTH + (BLOCK_SIZE * 10) + 1; i++)
	{
		statusCode = VectorAdd(vec, DATA_NUM);
	}
	
	currentSize = GetSizeOfVector(vec);
	
	for(i = 0; i < LENGTH + (BLOCK_SIZE * 10) + 1; i++)
	{
		statusCode = VectorDelete(vec, &tempNum);
		
		if(currentSize > GetSizeOfVector(vec))
		{
			hasChangedSizeOnDemand = 1;
		}
	}
	
	if(GetOriginalVectorSize(vec) == GetSizeOfVector(vec))
	{
		hasStoppedAtOriginalSize = 1;
	}
	
	if(hasChangedSizeOnDemand && hasStoppedAtOriginalSize && GetSizeOfVector(vec) == LENGTH && statusCode == ERR_OK)
	{
		printf("Test_ShrinkVector_VectorDelete: Passed!\n");
	}
	else
	{
		printf("Test_ShrinkVector_VectorDelete: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
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
	ADTErr statusCode;
	Vector* vec = NULL;
	int tempNum = -1, anotherTempNum = -1;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorGet(vec, 0, &tempNum)) == ERR_OK && tempNum == itemsArray[0] && (statusCode = VectorGet(vec, 1, &anotherTempNum)) == ERR_OK && anotherTempNum == itemsArray[1] && GetNumberOfItemsInVector(vec) == 2 /* Make sure not deleting items */)
	{
		printf("Test_Reg_VectorGet: Passed!\n");
	}
	else
	{
		printf("Test_Reg_VectorGet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_Null_VectorGet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int tempNum = -1;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	
	if((statusCode = VectorGet(NULL, 0, &tempNum)) == ERR_NOT_INITIALIZED && tempNum == -1 && (statusCode = VectorGet(vec, 1, NULL)) == ERR_NOT_INITIALIZED && (statusCode = VectorGet(NULL, 0, NULL)) == ERR_NOT_INITIALIZED)
	{
		printf("Test_Null_VectorGet: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorGet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_WrongIndex_VectorGet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int tempNum = -1, anotherTempNum = -1;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	
	if((statusCode = VectorGet(vec, TO_BIG_INDEX, &tempNum)) == ERR_WRONG_INDEX && tempNum == -1 && (statusCode = VectorGet(vec, LENGTH, &anotherTempNum)) == ERR_WRONG_INDEX && anotherTempNum == -1)
	{
		printf("Test_WrongIndex_VectorGet: Passed!\n");
	}
	else
	{
		printf("Test_WrongIndex_VectorGet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


/* VectorSet Tests: */
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
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(vec, 0, OTHER_DATA_NUM)) == ERR_OK && itemsArray[0] == OTHER_DATA_NUM && (statusCode = VectorSet(vec, 1, DATA_NUM)) == ERR_OK && itemsArray[1] == DATA_NUM && GetNumberOfItemsInVector(vec) == 2 /* Make sure not deleting items */)
	{
		printf("Test_Reg_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_Reg_VectorSet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_Null_VectorSet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(NULL, 0, OTHER_DATA_NUM)) == ERR_NOT_INITIALIZED && itemsArray[0] != OTHER_DATA_NUM)
	{
		printf("Test_Null_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorSet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_WrongIndex_VectorSet(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	int* itemsArray = NULL;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	
	itemsArray = GetVectorItemsArray(vec);
	
	if((statusCode = VectorSet(vec, TO_BIG_INDEX, OTHER_DATA_NUM)) == ERR_WRONG_INDEX && (statusCode = VectorSet(vec, LENGTH, OTHER_DATA_NUM)) == ERR_WRONG_INDEX && itemsArray[GetNumberOfItemsInVector(vec) - 1] != OTHER_DATA_NUM)
	{
		printf("Test_WrongIndex_VectorSet: Passed!\n");
	}
	else
	{
		printf("Test_WrongIndex_VectorSet: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


/* VectorGetItemsNumber Tests: */
void Test_VectorGetItemsNumber(void)
{
	Test_Reg_VectorGetItemsNumber();
	Test_Null_VectorGetItemsNumber();
}


void Test_Reg_VectorGetItemsNumber(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t itemsNum = 0;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, OTHER_DATA_NUM);
	
	if((statusCode = VectorGetItemsNumber(vec, &itemsNum)) == ERR_OK && itemsNum == 4)
	{
		printf("Test_Reg_VectorGetItemsNumber: Passed!\n");
	}
	else
	{
		printf("Test_Reg_VectorGetItemsNumber: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_Null_VectorGetItemsNumber(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t itemsNum = 0;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	VectorAdd(vec, DATA_NUM);
	VectorAdd(vec, DATA_NUM);
	
	if((statusCode = VectorGetItemsNumber(vec, NULL)) == ERR_NOT_INITIALIZED && (statusCode = VectorGetItemsNumber(NULL, &itemsNum)) == ERR_NOT_INITIALIZED && itemsNum == 0 && (statusCode = VectorGetItemsNumber(NULL, NULL) == ERR_NOT_INITIALIZED))
	{
		printf("Test_Null_VectorGetItemsNumber: Passed!\n");
	}
	else
	{
		printf("Test_Null_VectorGetItemsNumber: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


/* VectorUpdateExtensionBlockSize Tests: */
void Test_VectorUpdateExtensionBlockSize(void)
{
	Test_Reg_VectorUpdateExtensionBlockSize();
	Test_Null_VectorUpdateExtensionBlockSize();
}


void Test_Reg_VectorUpdateExtensionBlockSize(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t block = 100;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	statusCode = VectorUpdateExtensionBlockSize(vec, block);
	
	if(statusCode == ERR_OK && GetVectorExtensionBlockSize(vec) == block)
	{
		printf("Test_Reg_VectorUpdateExtensionBlockSize: Passed!\n");		
	}
	else
	{
		printf("Test_Reg_VectorUpdateExtensionBlockSize: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void Test_Null_VectorUpdateExtensionBlockSize(void)
{
	ADTErr statusCode;
	Vector* vec = NULL;
	size_t block = 100;

	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	statusCode = VectorUpdateExtensionBlockSize(NULL, block);
	
	if(statusCode == ERR_NOT_INITIALIZED && GetVectorExtensionBlockSize(vec) != block)
	{
		printf("Test_Null_VectorUpdateExtensionBlockSize: Passed!\n");		
	}
	else
	{
		printf("Test_Null_VectorUpdateExtensionBlockSize: Failed...\n");
		printf("-------> Error: ");
		PrintError(statusCode);
	}
	
	VectorDestroy(vec);
}


void GeneralTest_PrintVector(void)
{
	int i, limit = 20, offset = 10;
	int temp;
	Vector* vec = NULL;
	
	vec = VectorCreate(LENGTH, BLOCK_SIZE);
	
	for(i = 0; i < limit; i++)
	{
		VectorAdd(vec, (i + 1));
	}
	
	PrintAllVector(vec);
	
	for(i = 0; i < limit - offset; i++)
	{
		VectorDelete(vec, &temp);
		printf("%d\n", temp);
	}
	
	PrintAllVector(vec);
	
	VectorDestroy(vec);
}
