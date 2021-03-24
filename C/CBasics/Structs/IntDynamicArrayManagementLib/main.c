/* Libraries: */
#include <stdio.h>
#include <stdlib.h>
#include "Dynamic_Allocation_Int_Array_Management_Library.h"

#define LENGTH 3
#define BLOCK_SIZE 2
#define DATA_NUM 5
#define OTHER_DATA_NUM 10
#define TO_BIG_BLOCK_SIZE 1000000000

/* void PrintIntDynamicArray(IntDynamicArray* _IDA); */
void Test_DestroyDynamicIntArray(void);
void Test_CreateDynamicIntArray(void);
void Test_Regular_CreateDynamicIntArray(void);
void Test_NegSize_CreateDynamicIntArray(void);
void Test_NegBlockSIzeIncrement_CreateDynamicIntArray(void);
void Test_ZeroSizeZeroBlockSizeIncrement_CreateDynamicIntArray(void);
void Test_InsertToIntDynamicArray(void);
void Test_Reg_InsertToIntDynamicArray(void);
void Test_IncreaseMemory_InsertToIntDynamicArray(void);
void Test_OverFlow_InsertToIntDynamicArray(void);
void Test_BigBlockFailedReAllocation_InsertToIntDynamicArray(void);
void Test_RemoveFromIntDynamicArray(void);
void Test_Reg_RemoveFromIntDynamicArray(void);
void Test_UnderFlow_RemoveFromIntDynamicArray(void);


int main(void)
{	
	Test_CreateDynamicIntArray();
	Test_DestroyDynamicIntArray();
	Test_InsertToIntDynamicArray();
	Test_RemoveFromIntDynamicArray();
	
	return 0;
}

/*
void PrintIntDynamicArray(IntDynamicArray* _IDA)
{
	int i;
	
	putchar('\n');
	putchar('[');
	for(i = 0; i < _IDA->m_DynamicArraySize; i++)
	{
		if(i == _IDA->m_DynamicArraySize - 1)
		{
			printf("%d", _IDA->m_DynamicArrayPtr[i]);
		}
		else
		{
			printf("%d, ", _IDA->m_DynamicArrayPtr[i]);
		}
	}
	putchar(']');
	putchar('\n');
}
*/

void Test_CreateDynamicIntArray(void)
{
	Test_Regular_CreateDynamicIntArray();
	Test_NegSize_CreateDynamicIntArray();
	Test_NegBlockSIzeIncrement_CreateDynamicIntArray();
	Test_ZeroSizeZeroBlockSizeIncrement_CreateDynamicIntArray();
}

void Test_Regular_CreateDynamicIntArray(void)
{
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	if(!IDA)
	{
		printf("Test_Regular_CreateDynamicIntArray: Failed...\n");
		return;
	}
	
	/* Check inside the struct: */
	
	if(IDA->m_DynamicArrayPtr && IDA->m_DynamicArraySize == LENGTH && IDA->m_BlockSizeValueToIncrement == BLOCK_SIZE && IDA->m_NumOfElemsInDynamicArray == 0)
	{
		printf("Test_Regular_CreateDynamicIntArray: Passed!\n");
	}
	DestroyDynamicIntArray(IDA);
}

void Test_NegSize_CreateDynamicIntArray()
{
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(-1, BLOCK_SIZE);
	if(!IDA)
	{
		printf("Test_NegSize_CreateDynamicIntArray: Passed!\n");
	}
	else
	{
		printf("Test_NegSize_CreateDynamicIntArray: Failed...\n");
		DestroyDynamicIntArray(IDA);
	}
}

void Test_NegBlockSIzeIncrement_CreateDynamicIntArray()
{
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, -1);
	if(!IDA)
	{
		printf("Test_NegBlockSIzeIncrement_CreateDynamicIntArray: Passed!\n");
	}
	else
	{
		printf("Test_NegBlockSIzeIncrement_CreateDynamicIntArray: Failed...\n");
		DestroyDynamicIntArray(IDA);
	}
}

void Test_ZeroSizeZeroBlockSizeIncrement_CreateDynamicIntArray()
{
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(0, 0);
	if(!IDA)
	{
		printf("Test_ZeroSizeZeroBlockSizeIncrement_CreateDynamicIntArray: Passed!\n");
	}
	else
	{
		printf("Test_ZeroSizeZeroBlockSizeIncrement_CreateDynamicIntArray: Failed...\n");
		DestroyDynamicIntArray(IDA);
	}
}


void Test_InsertToIntDynamicArray(void)
{
	Test_Reg_InsertToIntDynamicArray();
	Test_IncreaseMemory_InsertToIntDynamicArray();
	Test_OverFlow_InsertToIntDynamicArray();
	Test_BigBlockFailedReAllocation_InsertToIntDynamicArray();
}


void Test_Reg_InsertToIntDynamicArray(void)
{
	int statusCode;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	
	statusCode = InsertToIntDynamicArray(IDA, DATA_NUM);
	
	if(statusCode == OK && IDA->m_NumOfElemsInDynamicArray == 1 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray - 1] == DATA_NUM)
	{
		statusCode = InsertToIntDynamicArray(IDA, OTHER_DATA_NUM);
		if(statusCode == OK && IDA->m_NumOfElemsInDynamicArray == 2 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray - 1] == OTHER_DATA_NUM)
		{
			printf("Test_Reg_InsertToIntDynamicArray: Passed!\n");
		}
		else
		{
			printf("Test_Reg_InsertToIntDynamicArray: [In second insertion:] Failed...\n");
		}
	}
	else
	{
		printf("Test_Reg_InsertToIntDynamicArray: [In first insertion:] Failed...\n");
	}
	
	DestroyDynamicIntArray(IDA);
}


void Test_IncreaseMemory_InsertToIntDynamicArray(void)
{
	int i, statusCode;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	
	for(i = 0; i < LENGTH; i++)
	{
		InsertToIntDynamicArray(IDA, DATA_NUM);
	}
	
	statusCode = InsertToIntDynamicArray(IDA, DATA_NUM); /* Should increase (reallocating) memory */
	
	if(statusCode == OK && IDA->m_NumOfElemsInDynamicArray == LENGTH + 1 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray - 1] == DATA_NUM && IDA->m_DynamicArraySize == LENGTH + BLOCK_SIZE)
	{
		printf("Test_IncreaseMemory_InsertToIntDynamicArray: Passed!\n");
	}
	else
	{
		printf("Test_IncreaseMemory_InsertToIntDynamicArray: Failed...\n");
	}
	
	DestroyDynamicIntArray(IDA);
}

void Test_OverFlow_InsertToIntDynamicArray(void)
{
	int i, statusCode;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, 0);
	
	for(i = 0; i < LENGTH + 1; i++)
	{
		InsertToIntDynamicArray(IDA, DATA_NUM);	
	}
	
	statusCode = InsertToIntDynamicArray(IDA, OTHER_DATA_NUM); /* Should fail (Over Flow) */
	
	if(statusCode == MEMORY_OVERFLOW_ERROR && IDA->m_NumOfElemsInDynamicArray == LENGTH && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray - 1] != OTHER_DATA_NUM && IDA->m_DynamicArraySize == LENGTH)
	{
		printf("Test_OverFlow_InsertToIntDynamicArray: Passed!\n");
	}
	else
	{
		printf("Test_OverFlow_InsertToIntDynamicArray: [In the last iteration:] Failed...\n");
	}
	
	DestroyDynamicIntArray(IDA);
}

void Test_BigBlockFailedReAllocation_InsertToIntDynamicArray(void)
{
	int i, statusCode;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(1, TO_BIG_BLOCK_SIZE);
	
	InsertToIntDynamicArray(IDA, DATA_NUM);
	
	statusCode = InsertToIntDynamicArray(IDA, OTHER_DATA_NUM);	

	if(statusCode == ALLOCATION_FAILED_ERROR && IDA->m_NumOfElemsInDynamicArray == 1 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray] != OTHER_DATA_NUM && IDA->m_DynamicArraySize == 1)
	{
		printf("Test_BigBlockFailedReAllocation_InsertToIntDynamicArray: Passed!\n");
	}
	else
	{
		printf("Test_BigBlockFailedReAllocation_InsertToIntDynamicArray: [In the last iteration:] Failed... (Optional: Successful ReAllocation of %d Bytes - Passed Regular Insertion)\n", TO_BIG_BLOCK_SIZE);
	}
	
	DestroyDynamicIntArray(IDA);
}

void Test_DestroyDynamicIntArray(void)
{
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	
	DestroyDynamicIntArray(IDA);
	
	if(IDA->m_MagicNumber == 0)
	{
		printf("Test_DestroyDynamicIntArray: Passed!\n");
	}
	else
	{
		printf("Test_DestroyDynamicIntArray: Failed...\n");
	}
}


void Test_RemoveFromIntDynamicArray(void)
{
	Test_Reg_RemoveFromIntDynamicArray();
	Test_Reg_RemoveFromIntDynamicArray();
	Test_UnderFlow_RemoveFromIntDynamicArray();
}


void Test_Reg_RemoveFromIntDynamicArray(void)
{
	int statusCode, tempNum;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	
	InsertToIntDynamicArray(IDA, DATA_NUM);
	InsertToIntDynamicArray(IDA, OTHER_DATA_NUM);
	
	if((statusCode = RemoveFromIntDynamicArray(IDA, &tempNum) == OK) && tempNum == OTHER_DATA_NUM && IDA->m_NumOfElemsInDynamicArray == 1 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray - 1] != OTHER_DATA_NUM)
	{
		if((statusCode = RemoveFromIntDynamicArray(IDA, &tempNum) == OK) && tempNum == DATA_NUM && IDA->m_NumOfElemsInDynamicArray == 0 && IDA->m_DynamicArrayPtr[0] != DATA_NUM)
		{
			printf("Test_Reg_RemoveFromIntDynamicArray: Passed!\n");	
		}
		else
		{
			printf("Test_Reg_RemoveFromIntDynamicArray: [In the second remove:] Failed...\n");
		}
	}
	else
	{
		printf("Test_Reg_RemoveFromIntDynamicArray: [In the first remove:] Failed...\n");	
	}
	
	DestroyDynamicIntArray(IDA);
}

void Test_UnderFlow_RemoveFromIntDynamicArray(void)
{
	int i, statusCode, tempNum;
	IntDynamicArray* IDA = NULL;
	
	IDA = CreateDynamicIntArray(LENGTH, BLOCK_SIZE);
	
	for(i = 0; i < LENGTH; i++)
	{
		InsertToIntDynamicArray(IDA, DATA_NUM);
	}
	
	for(i = LENGTH; i >= 0; i--)
	{
		RemoveFromIntDynamicArray(IDA, &tempNum);
	}
	
	if((statusCode = RemoveFromIntDynamicArray(IDA, &tempNum) == MEMORY_UNDERFLOW_ERROR && IDA->m_NumOfElemsInDynamicArray == 0 && IDA->m_DynamicArrayPtr[IDA->m_NumOfElemsInDynamicArray] != DATA_NUM))
	{
		printf("Test_UnderFlow_RemoveFromIntDynamicArray: Passed!\n");
	}
	else
	{
		printf("Test_UnderFlow_RemoveFromIntDynamicArray: Failed...\n");
	}
	
	DestroyDynamicIntArray(IDA);
}




