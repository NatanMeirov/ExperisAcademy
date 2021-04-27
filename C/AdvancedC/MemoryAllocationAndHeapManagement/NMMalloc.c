/**
 * @file NMMalloc.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Natan Meirov's Memory Allocation Manager Application Interface implementation for a shared object
 * @version 1.0
 * @date 2021-04-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include "NMMalloc.h"


/* Defines: */

typedef unsigned char Byte;

#define MINIMUM_MEMORY_CHUNK_IN_BYTES 32
#define MAX_SIZE_T ((size_t) -1)
#define BYTES_OF_HEADER 8

#define TURN_ON_BIT_N(type, variable, nthBit) ((variable) |= ((type)1 << ((nthBit) - 1)))
#define TURN_OFF_BIT_N(type, variable, nthBit) ((variable) &= ~((type)1 << ((nthBit) - 1)))
#define CHECK_BIT_N(variable, nthBit) ((variable) >> ((nthBit) - 1))

#define CHECK_SIGNATURE(byte) (CHECK_BIT_N((byte), 8)) /* Will be 1 if signed (MSB is 1), else will be 0 */
#define IS_ALLOCATED_BLOCK_ALREADY(header) (CHECK_BIT_N((header), 64)) /* Will be 1 if has allocated already (MSB is 1), else will be 0 */
#define SIGN_ON_BYTE(byte) (TURN_ON_BIT_N(Byte, (byte), 8))
#define SIGN_ON_HEADER_SO_IS_ALLOCATED(header) (TURN_ON_BIT_N(size_t, (header), 64))
#define REMOVE_SIGN_FROM_HEADER_SO_IS_FREE(header) (TURN_OFF_BIT_N(size_t, (header), 64))

#define ROUND_UP_NUMBER_TO_GET_DIVISOR_OF_EIGHT(number)\
while((number) % 8)\
{\
(number)++;\
}

#define GET_SIZE_OF_BLOCK_IN_BYTES(blockSizeInBytes, header)\
REMOVE_SIGN_FROM_HEADER_SO_IS_FREE((header));\
(blockSizeInBytes) = (header);\
SIGN_ON_HEADER_SO_IS_ALLOCATED((header));


/* -------------------------------- Main API Functions: ----------------------------------*/

void NMMallocManagerInitialization(void* _memoryToManage, size_t _memoryChunkSize)
{
    if(!_memoryToManage || _memoryChunkSize < MINIMUM_MEMORY_CHUNK_IN_BYTES)
    {
        return;
    }

    /* Saves the size of the memory chunk */
    *((size_t*)_memoryToManage) = _memoryChunkSize;

    /* Sign the block as managed by the NM Memory Allocation Manager */
    SIGN_ON_BYTE(*((Byte*)_memoryToManage));

    /* Initializes tail sentinel  */
    *(((size_t*)_memoryToManage) + _memoryChunkSize - 1) = MAX_SIZE_T;

    /* Initializes the first header to save the PREVIOUS BLOCK's available memory for the user's allocations */
    _memoryToManage = (((size_t*)_memoryToManage) + 1); /* Next address */
    *(size_t*)_memoryToManage = 0; /* There are 0 available bytes in the previous memory block */

    /* Initializes the second header (in a row) to save the NEXT available memory block for the user's allocations */
    _memoryToManage = (((size_t*)_memoryToManage) + 1); /* Next address */
    *(size_t*)_memoryToManage = _memoryChunkSize - 32; /* == NEXT block's bytes available for allocation */
    REMOVE_SIGN_FROM_HEADER_SO_IS_FREE((*(size_t*)_memoryToManage));
}


void* NMMalloc(void* _memoryChunk, size_t _requiredBlockSize)
{
    void* startingOfMemoryBlockToReturn;
    void* currentMemoryBlock;
    size_t blockSizeInBytes;
    char isFreePlaceFound = 0;
    size_t currentFreeMemoryBlockSizeInBytes;

    if(!_memoryChunk || !_requiredBlockSize)
    {
        return NULL;
    }

    ROUND_UP_NUMBER_TO_GET_DIVISOR_OF_EIGHT(_requiredBlockSize);

    /* Find a proper place for the allocation of the user using First Fit approach */
    currentMemoryBlock = ((size_t*)_memoryChunk + 2); /* Starting point */

    while(*(size_t*)currentMemoryBlock != MAX_SIZE_T)
    {
        if(!IS_ALLOCATED_BLOCK_ALREADY(*(size_t*)currentMemoryBlock))
        {
            if(*(size_t*)currentMemoryBlock >= _requiredBlockSize + (2 * BYTES_OF_HEADER)) /* 2 headers are required at every allocation */
            {
                currentFreeMemoryBlockSizeInBytes = *(size_t*)currentMemoryBlock;
                isFreePlaceFound = 1;
                break; /* Found */
            }
        }

        /* Check the next block */
        GET_SIZE_OF_BLOCK_IN_BYTES(blockSizeInBytes, (*(size_t*)currentMemoryBlock));
        currentMemoryBlock = (Byte*)currentMemoryBlock + BYTES_OF_HEADER /* Current header */ + blockSizeInBytes + BYTES_OF_HEADER; /* +8 to get the next header, and every header is 8 bytes (without it - we'll reach to the header that saves the size of the previous block from the back) */
    }

    if(!isFreePlaceFound) /* Wanted free place was not found */
    {
        return NULL;
    }

    /* Reached to a free block - update headers and return the start address of the block to the user */
    *(size_t*)currentMemoryBlock = _requiredBlockSize;
    SIGN_ON_HEADER_SO_IS_ALLOCATED(*(size_t*)currentMemoryBlock);
    startingOfMemoryBlockToReturn = (Byte*)currentMemoryBlock + BYTES_OF_HEADER;
    currentMemoryBlock = startingOfMemoryBlockToReturn + _requiredBlockSize; /* Reaches to the tail header */
    *(size_t*)currentMemoryBlock = _requiredBlockSize; /* Sets the allocated memory from the back */
    currentMemoryBlock = (size_t*)currentMemoryBlock + 1; /* Gets to the next header */
    *(size_t*)currentMemoryBlock = currentFreeMemoryBlockSizeInBytes - _requiredBlockSize - (2 * BYTES_OF_HEADER); /* Updates the remaining available memory block */

    return startingOfMemoryBlockToReturn;
}


void* NMCalloc(void* _memoryChunk, size_t _numberOfElements, size_t _sizeInBytesOfEachElement)
{
    /* if _numberOfElements == 0 or _sizeInBytesOfEachElement == 0 => NMMalloc will fail and return NULL */

    size_t bytesToClear;
    void* ptr = NMMalloc(_memoryChunk, _numberOfElements * _sizeInBytesOfEachElement);

    if(ptr)
    {
        bytesToClear = _sizeInBytesOfEachElement * _numberOfElements;

        do
        {
            bytesToClear--; /* (N - 1)...0 */
            ptr = (Byte*)ptr + bytesToClear;

            if(*((Byte*)ptr)) /* Only if is not 0: write 0 in the memory */
            {
                *((Byte*)ptr) = 0;
            }
        } while(bytesToClear); /* ptr should get back to its starting point (ptr + 0 [offset]) */
    }

    return ptr;
}


void NMFree(void* _blockToFree)
{

}

/* ------------------------------ End of Main API Functions -------------------------------*/