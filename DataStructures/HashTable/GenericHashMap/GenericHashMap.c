/**
 * @file GenericHashMap.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Hash Map of Key-Value pairs, implemented with separate chaining using LinkedLists
 * @details The HashMap (sometimes called: Dictionary or Associative Array),
 * is a set of unique (distinct) keys (or indexes) mapped (associated) to values
 * Size of allocated table will be the nearest prime number greater than the requested capacity
 * THe HashMap using lazy approach while allocating LinkedLists (that used for chaining)
 * @version 1.0
 * @date 2021-04-05
 *
 * @bug No known bugs
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, calloc, free */
#include <math.h> /* sqrt */
#include "GenericHashMap.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/GenericLinkedList.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/LinkedListIterator.h"
#include "../../LinkedList/GenericLinkedList/GenericLinkedListUsingIterator/LinkedListIteratorFunctions.h"


/* Defines: */

#define MAX_SIZE_T ((size_t) -1)

struct HashMap
{
    LinkedList** m_hashMap;
    HashFunction m_hashFunction;
    EqualityFunction m_keysEqualityFunction;
    size_t m_hashMapCapacity; /* Real HashMap capacity, overflow cannot be occurred because of the use with LinkedLists when collision in an insertion occurs */
    size_t m_numOfKeyValuePairsInHashMap; /* Number of key-value pairs in the table */
    /* HashMapStats m_hashMapStatistics; */
};

typedef void (*DestroyFunction)(void* _element);

typedef struct KeyValuePair
{
    void* m_key;
    void* m_value;
} KeyValuePair;

typedef struct KeyValuePairDestroyer
{
    DestroyFunction m_DestroyKeyFunction;
    DestroyFunction m_DestroyValueFunction;
} KeyValuePairDestroyer;

typedef struct KeyFinder
{
    void* m_keyToFind;
    EqualityFunction m_keysEqualityFunction;
} KeyFinder;


typedef struct ForEachWrapper
{
    KeyValueActionFunction m_userKeyValueActionFunction;
    void* m_userContext;
    size_t m_countOfTriggeredActions;
} ForEachWrapper;


/* Helper Function Declarations: */

static size_t FindNearestPrimeNumber(size_t _number);
static int IsPrimeNumber(size_t _number);
static HashMap* InitializeNewHashMap(HashMap* _map, size_t _capacityOfHashMap, HashFunction _hashFunction, EqualityFunction _keysEqualityFunction);
static int DestroyKeyAndValueOfSinglePair(void* _pair, void* _destroyerContext);
static void DestroySingleKeyValuePair(void* _pair);
static int IsKeyExists(void* _pair, void* _keyFinderObject);
static LinkedListIterator GetPlaceOfKeyValuePairInLinkedList(LinkedList* _list, const void* _keyToFind, EqualityFunction _keysEqualityFunction);
static int ExtractKeyValuePairAndTriggerAction(void* _pair, void* _ForeEachWrapperContext);


/* ------------------------------------------- Main API Functions ------------------------------------- */

HashMap* HashMapCreate(size_t _capacity, HashFunction _hashFunction, EqualityFunction _keysEqualityFunction)
{
    HashMap* newHashMap = NULL;

    if(_capacity < 2 || !_hashFunction || !_keysEqualityFunction)
    {
        return NULL;
    }

    newHashMap = malloc(sizeof(HashMap));
    if(!newHashMap)
    {
        return NULL;
    }

    return InitializeNewHashMap(newHashMap, _capacity, _hashFunction,_keysEqualityFunction);
}


void HashMapDestroy(HashMap** _map, void (*_destroyKey)(void* _key), void (*_destroyValue)(void* _value))
{
    size_t i;
    KeyValuePairDestroyer destroyer;
    int IsKeyAndValueDestroyRequired = 0; /* False */

    if(_map && *_map)
    {
        if(_destroyKey || _destroyValue) /* Atleast one of them is not NULL */
        {
            IsKeyAndValueDestroyRequired = 1; /* True */
            destroyer.m_DestroyKeyFunction = _destroyKey;
            destroyer.m_DestroyValueFunction = _destroyValue;
        }

        for(i = 0; i < (*_map)->m_hashMapCapacity; i++)
        {
            if((*_map)->m_hashMap[i]) /* If there is an initialized LinkedList */
            {
                if(IsKeyAndValueDestroyRequired)
                {
                    LinkedListIteratorForEach(LinkedListIteratorBegin((*_map)->m_hashMap[i]), LinkedListIteratorEnd((*_map)->m_hashMap[i]), &DestroyKeyAndValueOfSinglePair, (void*)&destroyer);
                }

                LinkedListDestroy(&((*_map)->m_hashMap[i]), &DestroySingleKeyValuePair);
            }
        }

        free((*_map)->m_hashMap);
        free(*_map);
        *_map = NULL;
    }
}


HashMapResult HashMapReHash(HashMap* _map, size_t _newCapacity)
{
    /* TODO */
    return HASHMAP_SUCCESS;
}


HashMapResult HashMapInsert(HashMap* _map, const void* _key, const void* _value)
{
    KeyValuePair* newKeyValuePair = NULL;
    LinkedListIterator iterator = NULL;
    size_t hashMapIndex;

    if(!_map || !_key)
    {
        return HASHMAP_UNINITIALIZED_ERROR;
    }

    hashMapIndex = _map->m_hashFunction(_key) % _map->m_hashMapCapacity;

    /* Check if the key is duplicated: */
    if(_map->m_hashMap[hashMapIndex]) /* If LinkedList is not initialized - key cannot be duplicated, so keeping this check block */
    {
        iterator = GetPlaceOfKeyValuePairInLinkedList(_map->m_hashMap[hashMapIndex], _key, _map->m_keysEqualityFunction);

        if(iterator) /* If iterator is not NULL -> key was found... */
        {
            return HASHMAP_KEY_DUPLICATE_ERROR;
        }
    }
    else /* Allocating new LinkedList */
    {
        _map->m_hashMap[hashMapIndex] = LinkedListCreate();
        if(!_map->m_hashMap[hashMapIndex])
        {
            return HASHMAP_ALLOCATION_ERROR;
        }
    }

    newKeyValuePair = malloc(sizeof(KeyValuePair));
    if(!newKeyValuePair)
    {
        return HASHMAP_ALLOCATION_ERROR;
    }

    newKeyValuePair->m_key = (void*)_key;
    newKeyValuePair->m_value = (void*)_value;

    if(LinkedListInsertTail(_map->m_hashMap[hashMapIndex], (void*)newKeyValuePair) != LINKEDLIST_SUCCESS) /* Can be only LinkedList allocation failed error */
    {
        free(newKeyValuePair); /* Destroys the newly created unused key-value pair */
        return HASHMAP_ALLOCATION_ERROR; /* Failed to allocate memory for a new LinkedList Node */
    }

    _map->m_numOfKeyValuePairsInHashMap++;

    return HASHMAP_SUCCESS;
}


HashMapResult HashMapRemove(HashMap* _map, const void* _searchKey, void** _pKey, void** _pValue)
{
    LinkedListIterator iterator = NULL;
    KeyValuePair* removedKeyValuePair = NULL;
    size_t hashMapIndex;

    if(!_map || !_searchKey || !_pKey || !_pValue)
    {
        return HASHMAP_UNINITIALIZED_ERROR;
    }

    if(_map->m_numOfKeyValuePairsInHashMap == 0) /* If there are not key-value pairs - key cannot be in the HashMap */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    hashMapIndex = _map->m_hashFunction(_searchKey) % _map->m_hashMapCapacity;

    if(!_map->m_hashMap[hashMapIndex]) /* If LinkedList is not initialized - key cannot be in the HashMap */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    iterator = GetPlaceOfKeyValuePairInLinkedList(_map->m_hashMap[hashMapIndex], _searchKey, _map->m_keysEqualityFunction);

    if(!iterator) /* If iterator is NULL -> key was not found... */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    removedKeyValuePair = (KeyValuePair*)LinkedListIteratorRemove(iterator);
    *_pKey = removedKeyValuePair->m_key;
    *_pValue = removedKeyValuePair->m_value;

    free(removedKeyValuePair); /* Destroys the removed key-value pair */
    _map->m_numOfKeyValuePairsInHashMap--;

    return HASHMAP_SUCCESS;
}


HashMapResult HashMapFind(const HashMap* _map, const void* _key, void** _pValue)
{
    LinkedListIterator iterator = NULL;
    size_t hashMapIndex;

    if(!_map || !_key || !_pValue)
    {
        return HASHMAP_UNINITIALIZED_ERROR;
    }

    if(_map->m_numOfKeyValuePairsInHashMap == 0) /* If there are not key-value pairs - key cannot be in the HashMap */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    hashMapIndex = _map->m_hashFunction(_key) % _map->m_hashMapCapacity;

    if(!_map->m_hashMap[hashMapIndex]) /* If LinkedList is not initialized - key cannot be in the HashMap */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    iterator = GetPlaceOfKeyValuePairInLinkedList(_map->m_hashMap[hashMapIndex], _key, _map->m_keysEqualityFunction);

    if(!iterator) /* If iterator is NULL -> key was not found... */
    {
        return HASHMAP_KEY_NOT_FOUND_ERROR;
    }

    *_pValue = ((KeyValuePair*)LinkedListIteratorGet(iterator))->m_value;

    return HASHMAP_SUCCESS;
}


size_t HashMapSize(const HashMap* _map)
{
    if(!_map)
    {
        return MAX_SIZE_T;
    }

    return _map->m_numOfKeyValuePairsInHashMap;
}


size_t HashMapForEach(const HashMap* _map, KeyValueActionFunction _actionFunction, void* _context)
{
    size_t i;
    ForEachWrapper wrapper;

    if(!_map || !_actionFunction)
    {
        return MAX_SIZE_T;
    }

    wrapper.m_userKeyValueActionFunction = _actionFunction;
    wrapper.m_userContext = _context;
    wrapper.m_countOfTriggeredActions = 0;

    for(i = 0; i < _map->m_hashMapCapacity; i++)
    {
        if(_map->m_hashMap[i])
        {
            if(LinkedListIteratorForEach(LinkedListIteratorBegin(_map->m_hashMap[i]), LinkedListIteratorEnd(_map->m_hashMap[i]), &ExtractKeyValuePairAndTriggerAction, (void*)&wrapper) == 0)
            {
                break;
            }
        }
    }

    return wrapper.m_countOfTriggeredActions;
}

/* --------------------------------------- End of Main API Functions ---------------------------------- */


/* Helper Function: */


static size_t FindNearestPrimeNumber(size_t _number)
{
    int foundPrime = 0;

    while(!foundPrime)
    {
        if(IsPrimeNumber(_number))
        {
            foundPrime = 1;
        }
        else
        {
            _number++;
        }
    }

    return _number;
}


static int IsPrimeNumber(size_t _number)
{
    size_t i;

    for(i = 2; i <= (size_t)sqrt((double)_number); i++)
    {
        if(_number % i == 0)
        {
            return 0; /* Is not prime */
        }
    }

    return 1; /* Is prime */
}


static HashMap* InitializeNewHashMap(HashMap* _map, size_t _capacityOfHashMap, HashFunction _hashFunction, EqualityFunction _keysEqualityFunction)
{
    size_t primeNumCapacityOfHashMap = FindNearestPrimeNumber(_capacityOfHashMap);

    _map->m_hashMap = calloc(primeNumCapacityOfHashMap, sizeof(LinkedList*)); /* Put NULLs on each LinkedList pointer */
    if(!_map->m_hashMap)
    {
        free(_map);
        return NULL;
    }

    _map->m_hashFunction = _hashFunction;
    _map->m_keysEqualityFunction = _keysEqualityFunction;

    _map->m_hashMapCapacity = primeNumCapacityOfHashMap;
    _map->m_numOfKeyValuePairsInHashMap = 0;

    return _map;
}


static int DestroyKeyAndValueOfSinglePair(void* _pair, void* _destroyerContext)
{
    /* Context is a KeyValuePairDestroyer object */

    if(((KeyValuePairDestroyer*)_destroyerContext)->m_DestroyKeyFunction)
    {
        ((KeyValuePairDestroyer*)_destroyerContext)->m_DestroyKeyFunction(((KeyValuePair*)_pair)->m_key);
    }

    if(((KeyValuePairDestroyer*)_destroyerContext)->m_DestroyValueFunction)
    {
        ((KeyValuePairDestroyer*)_destroyerContext)->m_DestroyValueFunction(((KeyValuePair*)_pair)->m_value);
    }

    return 1; /* To continue iterating */
}


static void DestroySingleKeyValuePair(void* _pair)
{
    free(_pair);
}


static int IsKeyExists(void* _pair, void* _keyFinderObject)
{
    return ((KeyFinder*)_keyFinderObject)->m_keysEqualityFunction(((KeyValuePair*)_pair)->m_key, ((KeyFinder*)_keyFinderObject)->m_keyToFind);
}


static LinkedListIterator GetPlaceOfKeyValuePairInLinkedList(LinkedList* _list, const void* _keyToFind, EqualityFunction _keysEqualityFunction)
{
    KeyFinder keyFinder;
    LinkedListIterator iterator = NULL;

    keyFinder.m_keyToFind = (void*)_keyToFind;
    keyFinder.m_keysEqualityFunction = _keysEqualityFunction;

    iterator = LinkedListIteratorFindFirst(LinkedListIteratorBegin(_list), LinkedListIteratorEnd(_list), &IsKeyExists, (void*)&keyFinder);

    if(iterator == LinkedListIteratorEnd(_list)) /* If iterator is placed on the end of the LinkedList -> key was not found */
    {
        return NULL;
    }

    return iterator;
}


static int ExtractKeyValuePairAndTriggerAction(void* _pair, void* _ForeEachWrapperContext)
{
    ((ForEachWrapper*)_ForeEachWrapperContext)->m_countOfTriggeredActions++;

    if(((ForEachWrapper*)_ForeEachWrapperContext)->m_userKeyValueActionFunction(((KeyValuePair*)_pair)->m_key, ((KeyValuePair*)_pair)->m_value, ((ForEachWrapper*)_ForeEachWrapperContext)->m_userContext) == 0 /* Should stop the whole iteration */)
    {
        return 0;
    }

    return 1; /* Continue the iteration */
}