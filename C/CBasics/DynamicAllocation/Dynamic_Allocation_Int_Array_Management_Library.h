/* Defines: */
#define OK 0
#define NULL_PTR_ERROR -10
#define ALLOCATION_FAILED_ERROR -11
#define MEMORY_OVERFLOW_ERROR -12
#define MEMORY_UNDERFLOW_ERROR -13
#define SIZE_ERROR -14

/*
- Description: Creates a dynamic array of integers

- Input: [_sizeOfArray:] the wanted size of the dynamic array 

- Output: A pointer to the dynamic array of integers, or NULL if error has occured

- Errors: ALLOCATION_FAILED_ERROR, SIZE_ERROR
*/
int* CreateDynamicIntArray(int _sizeOfArray);


/*
- Description: Destroys a dynamic array of integers (releasing the memory)

- Input: [_dynamicArrayPtr:] a pointer to a dynamic array of integers 

- Output: No Output

- Errors: NULL_PTR_ERROR
*/
void DestroyDynamicIntArray(int* _dynamicArrayPtr);


/*
- Description: Inserting a data integer to the end of the dynamic array, reallocates extern memory if needed (and returning the new address in a pointer to a pointer to the dynamic array of integers), calculates and returns the new dynamic array size and elements count in a pointers to an integers

- Input: [_ptrTodynamicArrayPtr:] a pointer to a poiner to a dynamic array of integers, [_dataToInsert:] an integer to insert in the end of the array (like append or push), [_dynamicArraySize:] the size of the dynamic array of integers, [_numOfElemsInDynamicArray:] number of the current elements count in the dynamic array of integers, [_blockSizeValueToIncrement:] a block size value to increment in the dynamic array

- Output: Status Code - [OK, NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR]

- Errors: NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR
*/
int InsertToIntDynamicArray(int** _ptrToDynamicArrayPtr, int _dataToInsert, int* _dynamicArraySize, int* _numOfElemsInDynamicArray, int _blockSizeValueToIncrement);


/*
- Description: Removing a data integer from the end of the dynamic array, returning the removed integer in a pointer to integer, calculates and returning the new elements count in a pointer to an integer

- Input: [_dynamicArrayPtr:] a pointer to a dynamic array of integers, [_removedData:] a pointer to an integer to store the removed data from the dynamic array of integers, [_numOfElemsInDynamicArray:] number of the current elements count in the dynamic array of integers

- Output: Status Code - [OK, NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_OVERFLOW_ERROR]

- Errors: NULL_PTR_ERROR, SIZE_ERROR, ALLOCATION_FAILED_ERROR, MEMORY_UNDERFLOW_ERROR
*/
int RemoveFromIntDynamicArray(int* _dynamicArrayPtr, int* _removedData, int* _numOfElemsInDynamicArray);



