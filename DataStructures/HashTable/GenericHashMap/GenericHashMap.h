/**
 * @file GenericHashMap.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Hash Map of Key-Value pairs, implemented with separate chaining using LinkedLists
 * @details The HashMap (sometimes called: Dictionary or Associative Array),
 * is a set of unique (distinct) keys (or indexes) mapped (associated) to values
 * Size of allocated HashMap will be the nearest prime number greater than the requested capacity
 * THe HashMap using lazy approach while allocating LinkedList that used for chaining
 * @version 1.0
 * @date 2021-04-05
 *
 * @bug No known bugs
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GENERICHASHMAP_H__
#define __GENERICHASHMAP_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

typedef struct HashMap HashMap;

typedef enum HashMapResult
{
    HASHMAP_SUCCESS = 0,
    HASHMAP_UNINITIALIZED_ERROR, /* Uninitialized HashMap error / Uninitialized key error */
    HASHMAP_KEY_DUPLICATE_ERROR, /* Duplicate key error */
    HASHMAP_KEY_NOT_FOUND_ERROR, /* Key not found error */
    HASHMAP_ALLOCATION_ERROR, /* Allocation error */
    HASHMAP_WRONG_CAPACITY_ERROR /* Initialize Capacity is less then 2 */
}HashMapResult;

/**
 * @brief A Hash function that returns the correct location of a given key
 * 
 */
typedef size_t (*HashFunction)(const void* _key);

/**
  * @brief A comparison function that returns true (none zero value) if a == b, or false (zero) otherwise
 * 
 */
typedef int (*EqualityFunction)(const void* _firstKey, const void* _secondKey);

/**
 * @brief An action callback function that will be called for each value in the HashMap
 * @param[in] _key: The Key that serves as an index for the given value
 * @param[in] _value: The Value that associates with the given key
 * @param[in] _context: A pointer to the context to pass to the action callback function, when the function is called
 * @return int - 0, if the iteration should stop on the current element / 1, if the iteration should continue to the next element
 */
typedef int (*KeyValueActionFunction)(const void* _key, void* _value, void* _context);


/* ------------------------------------------- Main API Functions ------------------------------------- */

/** 
 * @brief Dynamically creates a new HashMap object of a given capacity and key characteristics
 * @param[in] _capacity: HashMap's initial capacity, must be more then 2
 * @param[in] _hashFunction: A Hash function to hash each given key, cannot be NULL
 * @param[in] _keysEqualityFunction: Equality check function to check for equality of two keys, cannot be NULL
 * @return HashMap* - on success / NULL - on failure
 * @warning If _capacity is less then 2: function will fail and return NULL
 */
HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunction, EqualityFunction _keysEqualityFunction);


/** 
 * @brief Dynamically deallocates a previously allocated HashMap, NULLs the HashMap's pointer
 * @details Optionally can trigger given callback functions to destroy all keys and values using user provided functions (can provide both, just one, or none of them)
 * @param[in] _map: HashMap to be deallocated
 * @param[in] _destroyKey: A function pointer to be used to destroy each key in the HashMap
 * 			 			   or a NULL if no such destroy is required
 * @param[in] _destroyValue: A function pointer to be used to destroy each value in the HashMap
 * 			 			     or a NULL if no such destroy is required
 * @return None
 */
void HashMapDestroy(HashMap** _map, void (*_destroyKey)(void* _key), void (*_destroyValue)(void* _value));


/** 
 * @brief Adjust new HashMap capacity and ReHashes all key-value pairs
 * @param[in] _map: An existing HashMap to ReHash
 * @param[in] _newCapacity: new capacity shall be rounded to nearest larger prime number
 * @return HashMapResult - success or error status code
 * @retval HASHMAP_SUCCESS on success
 * @retval HASHMAP_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval HASHMAP_ALLOCATION_ERROR on error - allocation failed while trying to allocate memory
 * @retval HASHMAP_WRONG_CAPACITY_ERROR on error - given new initialize capacity is less then 2
 */
HashMapResult HashMapReHash(HashMap* _map, size_t _newCapacity);


/** 
 * @brief Inserts a key-value pair into the HashMap
 * @param[in] _map: HashMap to insert a key-value pair to
 * @param[in] _key: Key to serve as an index for the given value, cannot be NULL
 * @param[in] _value: Value to be associated with the given key, can be NULL
 * @return HashMapResult - success or error status code
 * @retval HASHMAP_SUCCESS on success
 * @retval HASHMAP_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval HASHMAP_KEY_DUPLICATE_ERROR on error - given key is already presented in the HashMap
 * @retval HASHMAP_ALLOCATION_ERROR on error - allocation failed while trying to allocate memory for the new key-value pair
 * 
 * @warning Each given key must be unique and destinct
 * @warning If _key is NULL: function will fail and return HASHMAP_UNINITIALIZED_ERROR
 */
HashMapResult HashMapInsert(HashMap* _map, const void* _key, const void* _value);


/** 
 * @brief Removes a key-value pair from the HashMap
 * @param[in] _map: HashMap to remove a key-value pair from
 * @param[in] _key: Key that serves as an index for a value, cannot be NULL
 * @param[in] _pKey: pointer to variable to return the key from HashMap by reference (if key was found), cannot be NULL
 * @param[in] _pValue: pointer to variable to return the value from HashMap by reference, cannot be NULL
 * @return HashMapResult - success or error status code
 * @retval HASHMAP_SUCCESS on success
 * @retval HASHMAP_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval HASHMAP_KEY_NOT_FOUND_ERROR on error - given key not found
 *
 * @warning Each given key must be unique and destinct
 * @warning If _searchKey or _pKey or _pValue are NULL: function will fail and return HASHMAP_UNINITIALIZED_ERROR
 */
HashMapResult HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue);


/** 
 * @brief Finds a value by giving its unique key
 * @param[in] _map: HashMap to find a value using a unique key from
 * @param[in] _key: Key that serves as an index for a value, cannot be NULL
 * @param[in] _pValue: pointer to variable to return the value from HashMap by reference, cannot be NULL
 * @return HashMapResult - success or error status code
 * @retval HASHMAP_SUCCESS on success
 * @retval HASHMAP_UNINITIALIZED_ERROR on error - given pointer is NULL
 * @retval HASHMAP_KEY_NOT_FOUND_ERROR on error - given key not found
 * 
 * @warning Each given key must be unique and destinct
 * @warning If _key or _pValue are NULL: function will fail and return HASHMAP_UNINITIALIZED_ERROR
 */
HashMapResult HashMapFind(const HashMap* _map, const void* _key, void** _pValue);


/**
 * @brief Gets the number of the key-value pairs in the HashMap
 * @param[in] _map: HashMap to get its key-value pairs number
 * @return size_t - number of key-value pairs in HashMap, on success / MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 */
size_t HashMapSize(const HashMap* _map);


/** 
 * @brief Iterates over all the key-value pairs in the HashMap, and triggers an action callback function on every single element
 * @details The user provides an action callback function that will be called for each key-value pair,
 *          if the action callback function returns 0 for an element - the iteration will stop,
 *			if the action callback function returns 1 for an element - the iteration will continue
 * @param[in] _map: HashMap to iterate over
 * @param[in] _actionFunction: User provided action callback function pointer to be invoked for each key-value pair
 * @param[in] _context: User provided context, that will be sent to the action callback function
 * @return size_t - number of times the user function was invoked (if successfully iterated all the key-value pairs of the HashMap), on success
			/ MAX_SIZE_T (-1 in size_t - is the maximum size_t value), on failure
 * 
 */
size_t HashMapForEach(const HashMap* _map, KeyValueActionFunction _actionFunction, void* _context);

/* --------------------------------------- End of Main API Functions ---------------------------------- */


/* --------------------------------------- Hash Map Statistic API Functions ---------------------------------- */

#ifndef __NDEBUG__
#define __NDEBUG__

typedef struct HashMapStats
{
    size_t numberOfBuckets; /* Number of LinkedLists created */
    size_t numberOfChains;
    size_t maxChainLength;
    size_t averageChainLength;
} HashMapStats;

HashMapStats HashMapGetStatistics(const HashMap* _map);

/* ------------------------------------ End of Hash Map Statistic API Functions -------------------------------- */


#endif /* #ifndef __NDEBUG__ */


#endif /* #ifndef __GENERICHASHMAP_H__ */
