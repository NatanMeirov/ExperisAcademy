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


#ifndef __GENERICVECTOR_HPP__
#define __GENERICVECTOR_HPP__

/* Includes: */

#include <stddef.h>  /* size_t */

/* Defines: */

struct Vector
{
    // Members
	void** m_vectorItems;
	size_t m_originalCapacity; /* Original allocated space for items - to avoid shrinking the Vector below its original size */
	size_t m_vectorCapacity; /* Actual allocated space for items */
	size_t m_sizeOfVector; /* Actual number of items in Vector */
	size_t m_extensionBlockSize; /* The chunk size to be allocated when no more space is available */
    void (*m_elementDestroyer)(void* a_element);

    // Vector Returned Status Codes
    enum Result
    {
        VECTOR_SUCCESS = 0,
        VECTOR_UNINITIALIZED_ERROR,				/* NULL pointer error */
        VECTOR_ALLOCATION_ERROR,				/* reallocation error on grow/shrink */
        VECTOR_UNDERFLOW_ERROR,
        VECTOR_OVERFLOW_ERROR,
        VECTOR_INDEX_OUT_OF_BOUNDS_ERROR,
        VECTOR_WRONG_SIZE_ERROR
    };

    typedef int	(*VectorElementAction)(void* a_element, size_t a_index, void* a_context);

    // C'tor
    Vector(size_t a_initialCapacity, size_t a_extensionBlockSize, void (*a_elementDestroyer)(void* a_element));

    // D'tor
    ~Vector();

    // Methods
    Result Append(void* a_element);
    Result Remove(void** a_pElement);
    Result Get(size_t _index, void** a_pElement);
    Result Set(size_t _index, void* a_element);
    size_t Size();
    size_t Capacity();
    size_t ForEach(VectorElementAction a_actionCallbackFunction, void* a_context);
};


/**
* @brief An action callback function that will be called for each element
* @param[in] a_element: A pointer to an element
* @param[in] a_index: The index of the element in the Vector
* @param[in] a_context: A pointer to the context to pass to the action callback function, when the function is called
* @return int - 0, if the iteration should stop on the current element / 1, if the iteration should continue to the next element
*/
// typedef int (*VectorElementAction)(void* a_element, size_t a_index, void* a_context);


/* ------------------------------------------- Main API Functions ------------------------------------ */

/**
 * @brief Dynamically creates a new Vector object of a given capacity and block size to increase when growing is needed
 * @param[in] a_initialCapacity: Vector's initial capacity, number of elements that can be stored initially
 * @param[in] a_extensionBlockSize: A block size that the Vector will grow or shrink on demand by
 * @param[in] a_elementDestroy: A function pointer to be used to destroy each element in the given Vector,
 * 			 				   or a NULL if no such destroy is required
 *
 * @warning If a_blockSize is 0: the Vector will be of fixed size, without growing option
 * @warning If both a_initialCapacity and a_blockSize are 0: function will fail
 */
// Vector(size_t a_initialCapacity, size_t a_extensionBlockSize, void (*_elementDestroy)(void* a_elementToDestroy));


// ~Vector();


/** 
 * @brief Appends (adds) an item to the end of the Vector
 * @param[in] a_item: An item to add, can be NULL
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_ALLOCATION_ERROR on error - failed to reallocate more memory
 * @retval VECTOR_OVERFLOW_ERROR on error - reached size limit, and extension block size is 0
 */
// Result Append(void* a_item);


/**  
 * @brief Removes (deletes) an item from the end of the Vector, NULLs the removed item's place
 * @param[out] a_pItem: A pointer to a variable that used to return the removed item's value by reference
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_UNDERFLOW_ERROR on error - empty, no items to remove
 *
 * @warning If _pItem is NULL: function will fail and return VECTOR_UNINITIALIZED_ERROR
 */
// Result Remove(void** a_pItem);


/**
 * @brief Gets the value of an item from the Vector, at a specific given index
 * @param[in] a_index: The index of the item to get its value from the Vector
 * @param[out] a_pItem: A pointer to a variable that used to return the wanted item's value by reference
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval VECTOR_INDEX_OUT_OF_BOUNDS_ERROR on error - given index is bigger then the index of the last item in the Vector
 *
 * @warning If _pItem is NULL: function will fail and return VECTOR_UNINITIALIZED_ERROR
 */
// Result Get(size_t a_index, void** a_pItem);


/**  
 * @brief Sets a new item at a specific given index of the Vector, to a new value
 * @param[in] a_index: The index to set the new item in the Vector
 * @param[in] a_itemToSet:  The new item value to set, can be NULL
 * @return VectorResult - success or error status code
 * @retval VECTOR_SUCCESS on success
 * @retval VECTOR_INDEX_OUT_OF_BOUNDS_ERROR on error - given index is bigger then the index of the last item in the Vector
 */
// Result Set(size_t a_index, void*  a_itemToSet);


/**  
 * @brief Gets the actual number of items in the Vector
 * @return size_t - number of items in Vector
 */
// size_t Size();


/**  
 * @brief Gets the current allocated space capacity for items of the Vector
 * @return size_t - capacity of Vector
 */
// size_t Capacity();


/**  
 * @brief Iterates over all the elements in the Vector, and triggers an action callback function on every single element
 * @details The user provides an action callback function that will be called for each element,
 *          if the action callback function returns 0 for an element - the iteration will stop,
 *			if the action callback function returns 1 for an element - the iteration will continue
 * @param[in] a_actionCallbackFunction: User provided action callback function pointer to be invoked for each element, cannot be NULL
 * @param[in] a_context: User provided context, that will be sent to action callback function
 * @return size_t - number of times the user function was invoked (if successfully iterated all the Vector [ == VectorSize]),
 			or the index of the element that the fuction was stopped on if not completed to iterate the whole Vector, on success
            / 0, on failure (a_actionCallbackFunction is NULL)
 *
 * A possible implementation of the function (using the API):
 *
 *      for(int i = 1; i < vector->Size(); ++i)
 *		{
 *             vector->Get(i, &elem);
 *             if(_action(elem, i, _context) == 0)
 *					break;
 *      }
 *		return i;
 */
// size_t ForEach(VectorElementAction _actionCallbackFunction, void* _context);

/* --------------------------------------- End of Main API Functions ---------------------------------- */


#endif // __GENERICVECTOR_HPP__