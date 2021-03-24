/**
 * @file SortingAlgorithms.h
 * @author Natan Meirov (NatanMeirov@dgmail.com)
 * @brief A Sorting Algorithms (for int Vector Data Structure) header file
 * @version 0.1
 * @date 2021-03-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __SORTINGALGORITHMS_H__
#define __SORTINGALGORITHMS_H__

/* Includes: */

#include "../../Dynamic_Vector/Vector.h"
#include "ADTDefs.h"


/*-------------------------------- Main API Functions: -----------------------------------------*/

/**
 * @brief Sorts a given Vector In-Place, using Bubble Sort Algorithm [Complexity: O(n^2), Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr BubbleSort(Vector* _vector);


/**
 * @brief Sorts a given Vector In-Place, using Shake Sort Algorithm [Complexity: O(n^2), Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr ShakeSort(Vector* _vector);


/**
 * @brief Sorts a given Vector In-Place, using Quick Sort Algorithm [Complexity: O(n*log(n)), Not Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr QuickSort(Vector* _vector);


/**
 * @brief Sorts a given Vector In-Place, using Insertion Sort Algorithm [Complexity: O(n^2), Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr InsertionSort(Vector* _vector);


/**
 * @brief Sorts a given Vector In-Place, using Shell Sort Algorithm [Complexity: O(n^4/3), Not Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr ShellSort(Vector* _vector);


/**
 * @brief Sorts a given Vector In-Place, using Selection Sort Algorithm [Complexity: O(n^2), Not Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr SelectionSort(Vector* _vector);


/**
* @brief Sorts a given Vector NOT In-Place, using Merge Sort Algorithm [Complexity: O(n*log(n)), Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr MergeSort(Vector* _vector);


/**
 * @brief Sorts a given Vector NOT In-Place, using Counting Sort Algorithm using Counting Sort Algorithm [Complexity: O(n), Optional: Stable or Not Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * @param _minValue: The value of the minimun item in the Vector
 * @param _maxValue: The value of the maximum item in the Vector
 * @param isStable: A boolean flag to optionally choose to use a stable sort or not
 * 
* @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr CountingSort(Vector* _vector, int _minValue, int _maxValue, int isStable);


/**
* @brief Sorts a given Vector NOT In-Place, using Radix Sort Algorithm [Complexity: O(n), Stable]
 * 
 * @param _vector: A pointer to a Vector to sort
 * @param _numberOfDigits: Number of digits of each item
 * 
 * @return [ADTErr] ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if Vector pointer is NULL)
 */
ADTErr RadixSort(Vector* _vector, size_t _numberOfDigits);

/*----------------------------- End of Main API Functions ---------------------------------------*/


#endif /* #ifndef __SORTINGALGORITHMS_H__ */