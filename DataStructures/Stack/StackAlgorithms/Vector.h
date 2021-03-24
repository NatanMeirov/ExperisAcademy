/* Vector header file */

#ifndef __VECTOR_H__ 
#define __VECTOR_H__

#include <stdlib.h> /* Used for size_t */
#include "ADTDefs.h"

typedef struct Vector Vector;

/*
- Description: Creates and returns a pointer to a newly created Vector, initialized with the wanted size value

- Input: [_initialSize:] The initial size value of the vector, can be 0, [_extensionBlockSize:] The value to extend the vector when there is no more space to insert more items, can be 0. Will fail if both [_initialSize] and [_extensionBlockSize] initialized with 0

- Output: A pointer to a Vector, or NULL is error has occured

- Errors: ERR_ALLOCATION_FAILED (if failed to allocate memory)
*/
Vector* VectorCreate(size_t _initialSize, size_t _extensionBlockSize);


/*
- Description: Destroys a Vector (by releasing its memory), validated no double release attempts

- Input: [_vector:] a pointer to a Vector 

- Output: No Output

- Errors: ERR_NOT_INITIALIZED (if null pointer)
*/
void VectorDestroy(Vector* _vector);


/*
- Description: Adds an item to the end of the Vector

- Input: [_vector:] a pointer to the vector, [_item:] item to add in the end of the vector

- Output: [ADTErr status code:] ERR_OK if succeed, else error 

- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_REALLOCATION_FAILED (if failed to reallocate memory), ERR_OVERFLOW (if reaches size limit, and extension block size is 0)
*/
ADTErr VectorAdd(Vector* _vector, int _item);


/*
- Description: Deletes an item from the end of the Vector

- Input: [_vector:] a pointer to the vector, [_item:] a pointer to a location to store the item in

- Output: [ADTErr status code:] ERR_OK if succeed, else error 

- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_UNDERFLOW (if no more items in vector)
*/
ADTErr VectorDelete(Vector* _vector, int* _item);



/*
- Description: Returns the item from the Vector, in the given index

- Input: [_vector:] a pointer to the vector, [_index:] the index to get the item from, [_item:] a pointer to a location to store the item to get

- Output: [ADTErr status code:] ERR_OK if succeed, else error 

- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_WRONG_INDEX (if index is bigger then number of current items - 1)
*/
ADTErr VectorGet(Vector* _vector, size_t _index, int* _item);


/*
- Description: Replaces the item from the Vector in the given index, by the given item

- Input: [_vector:] a pointer to the vector, [_index:] the index to get the item from, [_item:] an item to set

- Output: [ADTErr status code:] ERR_OK if succeed, else error 

- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_WRONG_INDEX (if index is bigger then number of current items - 1)
*/
ADTErr VectorSet(Vector* _vector, size_t _index, int _item);


/*
- Description: Returns the current number of items in the Vector

- Input: [_vector:] a pointer to the vector, [_numOfItems:] a pointer to a location to store the current number of items in the Vector

- Output: [ADTErr status code:] ERR_OK if succeed, else error

- Errors: ERR_NOT_INITIALIZED (if null pointer)
*/
ADTErr VectorGetItemsNumber(Vector* _vector, size_t* _numOfItems);


/*
- Description: Updates the extension block size to increment in the vector

- Input: [_vector:] a pointer to the vector, [_newExtensionBlockSize:] new extension block size to increment, when reallocation is needed to the vector

- Output: [ADTErr status code:] ERR_OK if succeed, else error

- Errors: ERR_NOT_INITIALIZED (if null pointer)
*/
ADTErr VectorUpdateExtensionBlockSize(Vector* _vector, size_t _newExtensionBlockSize);

#endif /* #ifndef __VECTOR_H__ */

