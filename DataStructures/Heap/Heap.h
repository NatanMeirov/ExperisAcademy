/**
 * @file Heap.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Maximum int Heap Data Structure header file
 * @version 0.1
 * @date 2021-03-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __HEAP_H__
#define __HEAP_H__

/* Includes: */

#include "ADTDefs.h"
#include "Vector.h"

/* Defines: */

typedef struct Heap Heap;

/*------------------------------------ Main API Functions: ----------------------------------------*/

/**
 * @brief Builds and returns a Heap from an existing (given) Vector - In place, validate not a NULL pointer to the Vector
 *  
 * @param _vector: A pointer to an existing Vector
 * 
 * @return [Heap*] A pointer to a newly created Heap if succeed, else NULL if error has occured (pointer is NULL)
 */
Heap* HeapBuild(Vector* _vector);



/**
 * @brief Destroys (releasing) a given Heap, prevents not a use of double destroy attempts (to avoid double free)
 * 
 * @param _heap: A pointer to a Heap to destroy
 */
void HeapDestroy(Heap* _heap);


/**
 * @brief Inserts a new data item to the heap, and puts it in the correct place
 * 
 * @param _heap: A pointer to a Heap
 * @param _data: A pointer to return the value of the max item by reference
 * 
 * @return [ADTErr] Status ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_OVERFLOW (if trying to insert an item to a full Vector), ERR_REALLOCATION_FAILED (if failed to reallocate more memory), ERR_OVERFLOW (if the Vector reached its size limit)
 */
ADTErr HeapInsert(Heap* _heap, int _data);


/**
 * @brief Returns the max item of the heap (head of the heap), returning its value by reference
 * 
 * @param _heap: A pointer to a Heap
 * @param _data: A pointer to return the value of the max item by reference
 * 
 * @return [ADTErr] Status ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_UNDERFLOW (if heap is empty)
 */
ADTErr HeapMax(Heap* _heap, int* _data);


/**
 * @brief Extracts the max item of the heap - from the heap, returning its value by reference
 * 
 * @param _heap: A pointer to a Heap
 * @param _data: A pointer to return the value of the max item by reference
 * 
 * @return [ADTErr] Status ERR_OK if succeed, else error: ERR_NOT_INITIALIZED (if pointer is NULL), ERR_UNDERFLOW (if heap is empty)
 */
ADTErr HeapExtractMax(Heap* _heap, int* _data);


/**
 * @brief Returns the Heap's items count
 * 
 * @param _heap: A pointer to a Heap
 * 
 * @return [int] Items count, or 0 if error has occured (pointer is NULL)
 */
size_t HeapGetItemsCount(Heap* _heap);


/**
 * @brief Prints a given Heap to stdout (screen)
 * 
 * @param _heap: A pointer to a Heap
 */
void HeapPrint(Heap* _heap);

/*---------------------------------- End of Main API Functions ----------------------------------------*/

#endif /* #ifndef __HEAP_H__ */