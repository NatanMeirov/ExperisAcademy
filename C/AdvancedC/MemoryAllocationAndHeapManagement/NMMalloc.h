/**
 * @file NMMalloc.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Natan Meirov's Memory Allocation Manager Application Interface implementation for a shared object
 * @version 1.0
 * @date 2021-04-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NMMALLOC_H__
#define __NMMALLOC_H__

/* Includes: */

#include <stddef.h> /* size_t */


/* -------------------------------- Main API Functions: ----------------------------------*/

/**
 * @brief Initializes a memory chunk to be managed by the NM Memory Allocation Manager,
 *        MUST be called BEFORE using other API function on the same memory chunk,
 *        Uses some bytes from the given memory chunk for inner management purposes,
 *        More then one given memory chunks can be managed at the same time
 * @param[in] _memoryToManage: An address of a memory chunk to be managed by the NMMALLOC
 * @param[in] _memoryChunkSize: The size of the given memory chunk to manage
 * @return None
 *
 * @warning After the initialization, only (_memoryChunkSize - 32) bytes will be available for the client usage
 */
void NMMallocManagerInitialization(void* _memoryToManage, size_t _memoryChunkSize);


/**
 * @brief Allocates free memory block from a given previously initialized memory chunk
 *        (managed already by the NMMallocManager after using NMMallocManagerInitialization function),
 *        Returns a pointer to the beggining of the wanted memory block from the memory chunk
 * @param[in] _memoryChunk: A previously initialized memory chunk, to allocate a memory block from
 * @param[in] _requiredBlockSize: The required block size in bytes to allocate from the given memory chunk
 * @return void* - A pointer to the beginning of the allocated memory block, on success
 *                 / NULL - on failure (optional reasons: not enough free memory space to allocate to the program,
                   the memory chunk is not a pre-initialized chunk by the NM Malloc Manager, given pointer is NULL)
 *
 * @warning Every allocation returns _requiredBlockSize bytes to the client, but reserves (_requiredBlockSize + 4) bytes from _memoryChunk
 */
void* NMMalloc(void* _memoryChunk, size_t _requiredBlockSize);


/**
 * @brief Allocates free memory block from a given previously initialized memory chunk
 *        (managed already by the NMMallocManager after using NMMallocManagerInitialization function),
 *        and clears the selected memory block,
 *        Returns a pointer to the beggining of the wanted cleared memory block from the memory chunk
 * @details The required block size would be (_numberOfElements * _sizeInBytesOfEachElement) bytes,
 *          The partition is required for performance optimizations
 * @param[in] _memoryChunk: A previously initialized memory chunk, to allocate a memory block from
 * @param[in] _numberOfElements: The required number of elements that the allocated memory should be filled with
 * @param[in] _sizeInBytesOfEachElement: The size in bytes of each element
 * @return void* - A pointer to the beginning of the allocated memory block, on success
 *                 / NULL - on failure (optional reasons: not enough free memory space to allocate to the program,
                   the memory chunk is not a pre-initialized chunk by the NM Malloc Manager, given pointer is NULL)
 *
 * @warning Every allocation returns (_numberOfElements * _sizeInBytesOfEachElement) bytes to the client, but reserves ((_numberOfElements * _sizeInBytesOfEachElement) + 8) bytes from _memoryChunk
 */
void* NMCalloc(void* _memoryChunk, size_t _numberOfElements, size_t _sizeInBytesOfEachElement);


/**
 * @brief Frees an allocated memory block, previously allocated by the NM Memory Allocation Manager through NMMalloc or NMCalloc functions,
 *        Prevents a try to free a memory block that was not allocated by the NM Memory Allocation Manager from the given memory chunk
 * @param[in] _blockToFree: A pointer to the block to free
 * @return None
 *
 * @warning Double free attempts can lead to an undefined behavior, and can mess the whole managed block
 * @warning There isn't a way to know if the given block has allocated by the NM Memory Allocation Manager
 */
void NMFree(void* _blockToFree);

/* ------------------------------ End of Main API Functions -------------------------------*/


#endif /* #ifndef __NMMALLOC_H__ */