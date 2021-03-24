/**
 * @file GenericHeap.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Creates a generic data type Binary Heap, over a generic data type Vector
 * @version 0.1
 * @date 2021-03-18
 *
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GENERICHEAP_H__
#define __GENERICHEAP_H__

/* Includes: */

#include "../../Vector/GenericVector/GenericVector.h"


/* Defines: */

typedef struct Heap Heap;

/* Heap Returned Status Codes */
typedef enum HeapResult
{
	HEAP_SUCCESS = 0,
	HEAP_NOT_INITIALIZED,                           /* Not Initialized Heap / Vector error or NULL pointer error */
	HEAP_REALLOCATION_FAILED,                       /* reallocation error on grow/shrink Vector */
	HEAP_OVERFLOW_ERROR
} HeapResult;

/**
 * @brief An action callback function that will be called for each element
 * @param[in] _element: A pointer to an element (cannot be modified - to avoid destroying the Heap's special order)
 * @param[in] _context: A pointer to the context to pass to the action callback function, when the function is called
 * @return int - 0 if the iteration should stop on the current element / 1 if the iteration should continue to the next element
 */
typedef int	(*ActionFunction)(const void *_elem, void * _context);

/**
 * @brief A comparison callback function that will be called to compare two elements in the Heap
 * @param[in] _left: A pointer to an element to check if its less then the other or not
 * @param[in] _right: A pointer to an element to check if its less then the other or not
 * @return int - 1, if the left element is less then the right element / 0, if the left element is not less then the right element
 */
typedef int	(*LessThanComparator)(const void *_left, const void *_right);


/* ------------------------------------------- Main API Functions ------------------------------------ */

/**  
 * @brief Dynamically creates a new Heap object from a given Vector, and a comparison callback function to build the Heap in a correct Heap order
 * @param[in] _vector: Vector that holds the elements, must be initialized
 * @param[in] _ptrFuncLessThen: A Less Than (<) comparison callback function, to be used to compare two elements
 * @return Heap* - on success / NULL - on failure
 * @warning Allocating and freeing the underlying Vector is the user responsibility
 */
Heap* HeapBuild(Vector* _vector, LessThanComparator _ptrFuncLessThen);


/**  
 * @brief Deallocates a previously allocated Heap,
 * 		  Frees the Heap and NULLs the Heap's pointer, but not the underlying vector
 * @param[in] _heap - Heap to be deallocated.  On success will be nulled.
 * @return Vector* - on success / NULL - on failure
 * @retval Underlying Vector
 */
Vector* HeapDestroy(Heap** _heap);


/**  
 * @brief Adds an element to the Heap, preserving the correct Heap's order property
 * @param[in] _heap: Heap to insert an element to
 * @param[in] _element: An element to insert to the Heap
 * @return HeapResult - success or error status code
 * @retval HEAP_SUCCESS on success
 * @retval HEAP_NOT_INITIALIZED on error - given pointer is NULL
 * @retval HEAP_REALLOCATION_FAILED on error - could not reallocate more memory for underlying Vector
 * @retval HEAP_OVERFLOW_ERROR on error - underlying Vector reached its size limit
 * @warning if underlying Vector or _element are NULL: function will fail and return HEAP_NOT_INITIALIZED
 */
HeapResult HeapInsert(Heap* _heap, void* _element);


/**  
 * @brief Peeks at the top element of the Heap, without extracting it
 * @param[in] _heap: Heap to peek into
 * @return const void* - A pointer to the top element of the Heap, on success / NULL - if Heap is empty, or on failure
 */
const void* HeapPeekTop(const Heap* _heap);


/**  
 * @brief Extracts the top element of the heap (removes it)
 * @param[in] _heap: Heap to extract its top item from
 * @return void* - A pointer to the top element of the Heap, on success / NULL - if Heap is empty, or on failure
 */
void* HeapExtract(Heap* _heap);


/**  
 * @brief Get the current size (number of elements) in the Heap
 * @param[in] _heap: Heap to get its actual number of items
 * @return size_t - number of items in Heap, on success / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t HeapSize(const Heap* _heap);


/**  
 * @brief Iterates over all the elements in the Heap, and triggers an action callback function on every single element
 * @details The user provides an action callback function that will be called for each element,
 *          if the action callback function returns 0 for an element - the iteration will stop,
 *			if the action callback function returns 1 for an element - the iteration will continue
 * @param[in] _heap: Heap to iterate over
 * @param[in] _action: User provided action callback function pointer to be invoked for each element
 * @param[in] _context: User provided context, that will be sent to action callback function
 * @return size_t - number of times the user function was invoked (if successfully iterated all the Vector [ == VectorSize]),
 			or the index of the element that the fuction was stopped on if not completed to iterate the whole Vector, on success
			/ MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t HeapForEach(const Heap* _heap, ActionFunction _action, void* _context);


/* --------------------------------------- End of Main API Functions ---------------------------------- */

#endif /* #ifndef __GENERICHEAP_H__ */
