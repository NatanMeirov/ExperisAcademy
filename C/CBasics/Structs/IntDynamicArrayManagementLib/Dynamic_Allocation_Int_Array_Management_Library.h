/* Defines: */
#define OK 0
#define NULL_PTR_ERROR -10
#define ALLOCATION_FAILED_ERROR -11
#define MEMORY_OVERFLOW_ERROR -12
#define MEMORY_UNDERFLOW_ERROR -13
#define SIZE_ERROR -14

typedef struct intDynamicArray
{
	int* m_DynamicArrayPtr;
	int m_DynamicArraySize;
	int m_BlockSizeValueToIncrement;
	int m_NumOfElemsInDynamicArray;
	int m_MagicNumber; /* Added in the code review */
} IntDynamicArray;

/*
- Description: Creates and returns an IntDynamicArray pointer, and initialize the struct of IntDynamicArray

- Input: [_sizeOfArray:] wanted initialize size of the dynamic array, [_BlockSizeValueToIncrement:] wanted initialize block size value to increment, when reallocation is needed

- Output: A pointer to a IntDynamicArray struct, or NULL if error has occured

- Errors: ALLOCATION_FAILED_ERROR, SIZE_ERROR
*/
IntDynamicArray* CreateDynamicIntArray(int _sizeOfArray, int _BlockSizeValueToIncrement);


/*
- Description: Destroys an IntDynamicArray (releasing the memory)

- Input: [_intDynamicArray:] a pointer to an IntDynamicArray struct 

- Output: No Output

- Errors: NULL_PTR_ERROR
*/
void DestroyDynamicIntArray(IntDynamicArray* _intDynamicArray);


/*
- Description: Inserts a new data integer to the end of the IntDynamicArray, and reallocates extern memory when needed

- Input: [_intDynamicArray:] a pointer to an IntDynamicArray struct, [_dataToInsert:] an integer to insert in the end of the array (like append or push)

- Output: Status Code - [OK, NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR]

- Errors: NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR
*/
int InsertToIntDynamicArray(IntDynamicArray* _intDynamicArray, int _dataToInsert);


/*
- Description: Removing a data integer from the end of the IntDynamicArray, returning the removed integer in a pointer to integer

- Input: [_intDynamicArray:] a pointer to an IntDynamicArray struct, [_removedData:] a pointer to an integer to store the removed data from the IntDynamicArray

- Output: Status Code - [OK, NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR]

- Errors: NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_UNDERFLOW_ERROR
*/
int RemoveFromIntDynamicArray(IntDynamicArray* _intDynamicArray, int* _removedData);


/*
- Description: Updates the block size value to increment every reallocation

- Input: [_newBlockSizeValue:] new block size value to increment, when reallocation is needed

- Output: Status Code - [OK, SIZE_ERROR]

- Errors: SIZE_ERROR
*/
int UpdateBlockSizeValueToIncrement(int _newBlockSizeValue);

