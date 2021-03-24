/* Generic Bubble Sort Header File */

#include <stddef.h> /* size_t */

/**
 * @brief A sort criteria callback function to trigger in the GenericBubbleSort, should return true (1) or false (0) if one element should be in a higher place then the other element, or not
 * 
 */
typedef int (*SortCriteria)(const void* _firstElm, const void* _secondElm);



/* -------------------------------------- Main API Function: --------------------------------------- */

/**
 * @brief A generic bubble sort, to sort an array of any type
 * 
 * @param _arrayToSort: A pointer to an array of any type to sort
 * @param _sizeOfArray: The size of the given array
 * @param _eachElementSizeInBytes: The size in bytes of each element in the given array
 * @param _criteria: A sort criteria callback function to trigger in the GenericBubbleSort, should return true (1) or false (0) if one element should be in a higher place then the other element, or not
 *
 * @return [int] 1 if the array sorted successfully, else 0 if error: NULL_PTR_ERROR (if a given pointer is NULL), ALLOCATION_ERROR (if failed to allocate new memory)
 */
int GenericBubbleSort(void* _arrayToSort, size_t _sizeOfArray, size_t _eachElementSizeInBytes, SortCriteria _criteria);

/* ----------------------------------- End of Main API Function ------------------------------------ */