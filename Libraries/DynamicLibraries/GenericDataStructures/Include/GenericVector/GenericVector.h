/**
 * @file GenericVector.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Vector data type that stores pointer to user provided elements of generic type
 * The Vector is heap allocated and can grow and shrink on demand
 * In version 2.0: Performance improvements
 * @version 2.0
 * @date 2021-03-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __GENERICVECTOR_H__
#define __GENERICVECTOR_H__

/* Includes: */

#include <stddef.h>  /* size_t */

/* Defines: */

typedef struct Vector Vector;


/* Vector Returned Status Codes */
typedef enum VectorResult
{
	VECTOR_SUCCESS = 0,
	VECTOR_UNINITIALIZED_ERROR,				/* Not Initialized Vector error or NULL pointer error */
	VECTOR_ALLOCATION_ERROR,				/* reallocation error on grow/shrink */
	VECTOR_UNDERFLOW_ERROR,
	VECTOR_OVERFLOW_ERROR,
	VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
	VECTOR_WRONG_SIZE_ERROR
} VectorResult;

/**
 * @brief An action callback function that will be called for each element
 * @param[in] _element: A pointer to an element
 * @param[in] _index: The index of the element in the Vector
 * @param[in] _context: A pointer to the context to pass to the action callback function, when the function is called
 * @return int - 0, if the iteration should stop on the current element / 1, if the iteration should continue to the next element
 */
typedef int	(*VectorElementAction)(void* _element, size_t _index, void* _context);


/* ------------------------------------------- Main API Functions ------------------------------------ */

/**
 * @brief Dynamically creates a new Vector object of a given capacity and block size to increase when growing is needed
 * @param[in] _initialCapacity: Vector's initial capacity, number of elements that can be stored initially
 * @param[in] _extensionBlockSize: A block size that the Vector will grow or shrink on demand by
 * @return Vector* - on success / NULL - on failure
 *
 * @warning If _blockSize is 0: the Vector will be of fixed size, without growing option
 * @warning If both _initialCapacity and _blockSize are 0: function will fail and return NULL
 */
Vector* VectorCreate(size_t _initialCapacity, size_t _extensionBlockSize);


/**  
 * @brief Dynamically deallocates a previously allocated Vector, NULLs the Vector's pointer
 * @param[in] _vector: Vector to be deallocated
 * @param[in] _elementDestroy: A function pointer to be used to destroy each element in the given Vector,
 * 			 				   or a NULL if no such destroy is required
 * @return None
 */
void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item));


/**  
 * @brief Appends (adds) an item to the end of the Vector
 * @param[in] _vector: A Vector to append an item to
 * @param[in] _item: An item to add, can be NULL
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_ALLOCATION_ERROR on error - failed to reallocate more memory
 * @retval VECTOR_OVERFLOW_ERROR on error - reached size limit, and extension block size is 0
 */
VectorResult VectorAppend(Vector* _vector, void* _item);


/**  
 * @brief Removes (deletes) an item from the end of the Vector, NULLs the removed item's place
 * @param[in] _vector: Vector to remove an item from
 * @param[out] _pItem: A pointer to a variable that used to return the removed item's value by reference
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_UNDERFLOW_ERROR on error - empty, no items to remove
 *
 * @warning If _pItem is NULL: function will fail and return VECTOR_UNINITIALIZED_ERROR
 */
VectorResult VectorRemove(Vector* _vector, void** _pItem);


/**  
 * @brief Gets the value of an item from the Vector, at a specific given index
 * @param[in] _vector: Vector to get the item from
 * @param[in] _index: The index of the item to get its value from the Vector
 * @param[out] _pItem: A pointer to a variable that used to return the wanted item's value by reference
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_INDEX_OUT_OF_BOUNDS_ERROR on error - given index is bigger then the index of the last item in the Vector
 *
 * @warning If _pItem is NULL: function will fail and return VECTOR_UNINITIALIZED_ERROR
 */
VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pItem);


/**  
 * @brief Sets a new item at a specific given index of the Vector, to a new value
 * @param[in] _vector: Vector to set the new item to
 * @param[in] _index: The index to set the new item in the Vector
 * @param[in] _itemToSet:  The new item value to set, can be NULL
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_INDEX_OUT_OF_BOUNDS_ERROR on error - given index is bigger then the index of the last item in the Vector
 */
VectorResult VectorSet(Vector* _vector, size_t _index, void*  _itemToSet);


/**  
 * @brief Gets the actual number of items in the Vector
 * @param[in] _vector: Vector to get its actual number of items
 * @return size_t - number of items in Vector, on success / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t VectorSize(const Vector* _vector);


/**  
 * @brief Gets the current allocated space capacity for items of the Vector
 * @param[in] _vector: Vector to get its actual capacity
 * @return size_t - capacity of Vector, on success / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t VectorCapacity(const Vector* _vector);


/**  
 * @brief Updates the extension block size to increase when growing, in the Vector
 * @param[in] _vector: Vector to update its extension block size
 * @param _newExtensionBlockSize: The new block size that the Vector will grow or shrink on demand by
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_WRONG_SIZE_ERROR on error - given new block size is 0, while Vector's capacity is 0 as well
 *
 * @warning If both _newExtensionBlockSize is 0, and current Vector capacity is 0 as well: function will fail and return VECTOR_WRONG_SIZE_ERROR
 */
VectorResult VectorUpdateExtensionBlockSize(Vector* _vector, size_t _newExtensionBlockSize);


/**  
 * @brief Iterates over all the elements in the Vector, and triggers an action callback function on every single element
 * @details The user provides an action callback function that will be called for each element,
 *          if the action callback function returns 0 for an element - the iteration will stop,
 *			if the action callback function returns 1 for an element - the iteration will continue
 * @param[in] _vector: Vector to iterate over
 * @param[in] _actionCallbackFunction: User provided action callback function pointer to be invoked for each element
 * @param[in] _context: User provided context, that will be sent to action callback function
 * @return size_t - number of times the user function was invoked (if successfully iterated all the Vector [ == VectorSize]),
 			or the index of the element that the fuction was stopped on if not completed to iterate the whole Vector, on success
			/ MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 *
 * A possible implementation of the function (using the API):
 *
 *      for(i = 1; i < VectorSize(v); ++i)
 *		{
 *             VectorGet(v, i, &elem);
 *             if(_action(elem, i, _context) == 0)
 *					break;
 *      }
 *		return i;
 */
size_t VectorForEach(const Vector* _vector, VectorElementAction _actionCallbackFunction, void* _context);

/* --------------------------------------- End of Main API Functions ---------------------------------- */


#endif /* #ifndef __GENERICVECTOR_H__ */
