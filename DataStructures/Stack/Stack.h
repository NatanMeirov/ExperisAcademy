/* Stack header file */

#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h> /* Used for size_t */
#include "ADTDefs.h"

typedef struct Stack Stack;

/*
- Description: Creates and returns a pointer to a Stack
- Input: [_size:] a size to initialize the stack's base size, [_blockSize:] a block size to extend the stack every time it needs to grow (when the stack is full). If both are initialized with 0 - error will occur
- Output: Pointer to stack if ERR_OK, else NULL if error has occured  
- Errors: ERR_WRONG_SIZE (when both _size and _blockSize initialized with 0), ERR_NOT_INITIALIZED (if null pointer)
*/
Stack* StackCreate(size_t _size, size_t _blockSize);


/*
- Description: Destroys a Stack
- Input: [_stack:] a pointer to a Stack
- Output: No output
- Errors: ERR_NOT_INITIALIZED (if null pointer)
*/
void StackDestroy(Stack* _stack);


/*
- Description: Pushes an item to the Stack's top
- Input: [_stack:] a pointer to a Stack, [_item:] an item to push to the top of the Stack
- Output: [ADTErr status code:] ERR_OK if succeed, else error 
- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_REALLOCATION_FAILED (if failed to reallocate memory), ERR_OVERFLOW (if reaches size limit, and extension block size is 0)
*/
ADTErr StackPush(Stack* _stack, int _item);


/*
- Description: Pops an item out of from the Stack's top
- Input: [_stack:] a pointer to a Stack, [_item:] an address of an item to store the poped item
- Output: [ADTErr status code:] ERR_OK if succeed, else error 
- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_UNDERFLOW (if no more items in vector)
*/
ADTErr StackPop(Stack* _stack, int* _item);


/*
- Description: Returns the value of the Stack's top item
- Input: [_stack:] a pointer to a Stack, [_item:] an address of an item to store the top item's value
- Output: [ADTErr status code:] ERR_OK if succeed, else error 
- Errors: ERR_NOT_INITIALIZED (if null pointer), ERR_UNDERFLOW (if there are no items in the Stack [Stack is empty])
*/
ADTErr StackTop(Stack* _stack, int* _item);


/*
- Description: Check if the Stack is empty, or not
- Input: [_stack:] a pointer to a Stack
- Output: 1 (true) - if the Stack is empty, 0 (false) - if the stack is not empty, else -1 (error) - if error has occured 
- Errors: ERR_NOT_INITIALIZED (if null pointer)
*/
int StackIsEmpty(Stack* _stack);


#endif /* #ifndef __STACK_H__ */
