/* Status Codes */
#define OK 0
#define SIZE_ERROR -10
#define NULL_PTR_ERROR -11

/*
Description: Finds the most common number in an array of integers (without changing the given array), returning it using a pointer
Input: [_array:] an array of integers, [_sizeOfArray:] size of the array, [_mostFrequentNum:] a pointer to an integer, to point in the frequent number (or just the address of the variable instead)
Output: Status code: OK, NULL_PTR_ERROR, SIZE_ERROR
Errors: NULL_PTR_ERROR, SIZE_ERROR
*/
int FindMostFrequentNumInArray(int* _array, int _sizeOfArray, int* _mostFrequentNum);


/*
Description: Reorder an array of integers (in place), while shifting even numbers to left (without changing their inner order) and shifting odd numbers to right (without changing their inner order)
Input: [_array:] an array of integers, [_sizeOfArray:] size of the array
Output: Number of even numbers in the array, or status code - NULL_PTR_ERROR, SIZE_ERROR, if error
Errors: NULL_PTR_ERROR, SIZE_ERROR
*/
int ReorderArrayAsEvensToLeftOddsToRight(int* _array, int _sizeOfArray);


/*
Description: Sorts an array of integers (in place)
Input: [_array:] an array of integers, [_sizeOfArray:] size of the array
Output: Status code: OK, NULL_PTR_ERROR, SIZE_ERROR
Errors: NULL_PTR_ERROR, SIZE_ERROR
*/
int BubbleSort(int* _array, int _sizeOfArray);


/*
Description: Finds the index of a given key in a sorted array [Complexity: O(log n)]
Input: [_array:] a sorted array of integers, [_sizeOfArray:] size of the array, [_key:] the number to search in the array
Output: The index of the found key (if there are more then one, then the first index), -1 if not found, or status code -NULL_PTR_ERROR, SIZE_ERROR, if error
Errors: NULL_PTR_ERROR, SIZE_ERROR
*/
int BinarySearch(int* _array, int _sizeOfArray, int _key);



