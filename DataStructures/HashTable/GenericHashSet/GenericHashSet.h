/**
 * @file GenericHashSet.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Hash Set data type that stores pointer to user provided elements of generic type
 * The Hash Set is heap allocated
 * @version 1.0
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __GENERICHASHSET_H__
#define __GENERICHASHSET_H__

/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

typedef struct HashSet HashSet;

typedef enum HashSetResult
{
    HASHSET_SUCCESS = 0,
    HASHSET_UNINITIALIZED, /* Uninitialized Hash Set error or given element is NULL */
    HASHSET_KEY_DUPLICATE, /* Duplicate key error */
    HASHSET_KEY_NOT_FOUND, /* Key not found */
    HASHSET_OVERFLOW /* No more space in Hash Set */
} HashSetResult;

/**
 * @brief A Hash function that returns the correct place of a given key
 * 
 */
typedef size_t (*HashFunction)(void* _element);

/**
 * @brief A ReHash function that returns the next correct place of a given key when the original Hash function failed to find an empty place
 * 
 */
typedef size_t (*ReHashFunction)(size_t _hashSetCurrentFullIndex, void* _context);

/**
 * @brief A comparison function that returns true (none zero value) if a == b, or false (zero) otherwise
 * 
 */
typedef int (*EqualityFunction)(void* _firstElement, void* _secondElement);


/* ------------------------------------------- Main API Functions ------------------------------------- */

/* TODO: finish the header file by writing API Functions description */

/**
 * @brief Creates a heap allocated Hash Set
 * 
 * @param[in] _size: Size of the new Hash Set, must be more then 2
 * @param[in] _hashFunction: Hash function
 * @param[in] _reHashFunction: ReHash function
 * @param[in] _reHashFunctionContext: ReHash function's context
 * @param[in] _equalityFunction: Equality function to check if two elements are the same (used to prevent duplicates while inserting, and while finding and removing)
 * @return HashSet* - on success / NULL - on failure
 */
HashSet* HashSetCreate(size_t _size, HashFunction _hashFunction, ReHashFunction _reHashFunction, void* _reHashFunctionContext, EqualityFunction _equalityFunction);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @param[in] _destroyElement 
 */
void HashSetDestroy(HashSet** _hash, void (*_destroyElement)(void* _element));


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @param[in] _element 
 * @return HashSetResult 
 */
HashSetResult HashSetInsert(HashSet* _hash, void* _element);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @param[in] _key 
 * @param[out] _pElement 
 * @return HashSetResult 
 */
HashSetResult HashSetRemove(HashSet* _hash, void* _key, void** _pElement);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @param[in] _key 
 * @return int 
 */
int HashSetIsFoundKey(const HashSet* _hash, void* _key);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @return size_t 
 */
size_t HashSetGetNumOfElements(const HashSet* _hash);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @return size_t 
 */
size_t HashSetGetCapacity(const HashSet* _hash);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @return double 
 */
double HashSetGetAverageAmountOfReHashOperations(const HashSet* _hash);


/**
 * @brief 
 * 
 * @param[in] _hash 
 * @return size_t 
 */
size_t HashSetGetTotalCountOfReHashOperations(const HashSet* _hash);

/* --------------------------------------- End of Main API Functions ---------------------------------- */


#endif /* #ifndef __GENERICHASHSET_H__ */