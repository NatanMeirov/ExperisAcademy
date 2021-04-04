/**
 * @file GenericHashSet.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Create a Generic Hash Set data type that stores pointer to user provided elements of generic type
 * The Hash Set is heap allocated
 * @version 1.0
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, calloc, free */
#include <math.h> /* sqrt */
#include "GenericHashSet.h"


/* Defines: */

#define MAX_SIZE_T ((size_t) -1)
#define REFACTORING_RATE (1.30)
#define SINGLE_BYTE (1)
#define KEY_FOUND (1)
#define KEY_NOT_FOUND (0)
#define EMPTY_PLACE (0)

struct HashSet
{
    void** m_hashSet;
    /* char* m_state; -> instead of wasting a lot of memory for a states array (a state for a each single place in hash - if empty, full or was full before - we will use a DUMMY pointer placed IN THE HASH TABLE to mark a place that was initialized before, but now empty (my own idea - I guess better then the previous idea): empty will be NULL, full will have its key, removed item will be init with dummy key till user enters new real key */
    void* m_dummyKey;
    HashFunction m_hashFunction;
    ReHashFunction m_reHashFunction;
    void* m_reHashFunctionContext;
    EqualityFunction m_equalityFunction;
    size_t m_hashSetOriginalSize; /* original size given by the client */
    size_t m_hashSetCapacity; /* real hash size */
    size_t m_numOfElementsInHashSet; /* number of occupied places in the table */
    size_t m_maxAmountOfReHashOperationsInARow; /* maximum amount of rehash operations which have been done over one insertion */
    size_t m_totalCountOfInsertions; /* total amount of insertions into the table */
    size_t m_totalCountOfReHashOperations; /* all rehash operations mutual counter */
};


/* Helper Function Declarations: */

static size_t RefactorHashSetSize(size_t _size);
static size_t FindNearestPrimeNumber(size_t _number);
static int IsPrimeNumber(size_t _number);
static HashSet* InitializeNewHashSet(HashSet* _hash, size_t _sizeOfHashSet, HashFunction _hashFunction, ReHashFunction _reHashFunction, void* _reHashFunctionContext, EqualityFunction _equalityFunction);
static size_t DefaultReHashFunction(size_t _hashSetIndex, void* _hashSetCapacity); /* Default ReHash Function - can be modified in the future to improve the ReHash logic */
static size_t GetPlaceOfElementInHashSet(const HashSet* _hash, void* _element, size_t _hashSetIndex);
static int IsKeyExist(void* _hashSetElement, void* _newElement, EqualityFunction _equalityFunction);


/* ------------------------------------------- Main API Functions ------------------------------------- */

HashSet* HashSetCreate(size_t _size, HashFunction _hashFunction, ReHashFunction _reHashFunction, void* _reHashFunctionContext, EqualityFunction _equalityFunction)
{
    HashSet* newHashSet = NULL;

    if(!_hashFunction || !_equalityFunction)
    {
        return NULL;
    }

    newHashSet = malloc(sizeof(HashSet));
    if(!newHashSet)
    {
        return NULL;
    }

    return InitializeNewHashSet(newHashSet, _size, _hashFunction, _reHashFunction, _reHashFunctionContext, _equalityFunction);
}


void HashSetDestroy(HashSet** _hash, void (*_destroyElement)(void* _element))
{
    size_t i, destroyedElements = 0;

    if(_hash && *_hash)
    {
        if(_destroyElement)
        {
            for(i = 0; i < (*_hash)->m_hashSetCapacity && destroyedElements < (*_hash)->m_numOfElementsInHashSet; i++)
            {
                if((*_hash)->m_hashSet[i] && (*_hash)->m_hashSet[i] != (*_hash)->m_dummyKey)
                {
                    _destroyElement((*_hash)->m_hashSet[i]);
                    destroyedElements++;
                }
            }
        }

        free((*_hash)->m_hashSet);
        free((*_hash)->m_dummyKey);
        free(*_hash);
        *_hash = NULL;
    }
}


HashSetResult HashSetInsert(HashSet* _hash, void* _element)
{
    HashSetResult statusCode;
    size_t hashSetIndex, reHashesNeeded = 0;

    if(!_hash || !_element)
    {
        return HASHSET_UNINITIALIZED;
    }

    if(_hash->m_numOfElementsInHashSet == _hash->m_hashSetOriginalSize)
    {
        return HASHSET_OVERFLOW;
    }

    hashSetIndex = (_hash->m_hashFunction(_element)) % _hash->m_hashSetCapacity;

    /* Using GetPlaceOfElement function to check if duplicate - if not MAX_SIZE_T has returned => Element is duplicated (MAX_SIZE_T will be returned if did not found) */
    if(GetPlaceOfElementInHashSet(_hash, _element, hashSetIndex) != MAX_SIZE_T)
    {
        return HASHSET_KEY_DUPLICATE;
    }

    while(!EMPTY_PLACE)
    {
        if(!(_hash->m_hashSet[hashSetIndex]) || _hash->m_hashSet[hashSetIndex] == _hash->m_dummyKey) /* If empty place or place with dummy key */
        {
            break; /* Found a place to insert */
        }

        /* Place is not empty - ReHash is needed */
        hashSetIndex = _hash->m_reHashFunction(hashSetIndex, _hash->m_reHashFunctionContext);
        _hash->m_totalCountOfReHashOperations++;
        reHashesNeeded++;
    }

    if(_hash->m_maxAmountOfReHashOperationsInARow < reHashesNeeded)
    {
        _hash->m_maxAmountOfReHashOperationsInARow = reHashesNeeded;
    }

    _hash->m_hashSet[hashSetIndex] = _element;
    _hash->m_totalCountOfInsertions++;
    _hash->m_numOfElementsInHashSet++;

    return HASHSET_SUCCESS;
}


HashSetResult HashSetRemove(HashSet* _hash, void* _key, void** _pElement)
{
    size_t hashSetIndex, i, placeOfElementToRemove;

    if(!_hash || !_key || !_pElement)
    {
        return HASHSET_UNINITIALIZED;
    }

    if(_hash->m_numOfElementsInHashSet == 0)
    {
        return HASHSET_KEY_NOT_FOUND;
    }

    hashSetIndex = _hash->m_hashFunction(_key) % _hash->m_hashSetCapacity;

    placeOfElementToRemove = GetPlaceOfElementInHashSet(_hash, _key, hashSetIndex);

    if(placeOfElementToRemove == MAX_SIZE_T)
    {
        return HASHSET_KEY_NOT_FOUND;
    }

    /* Removing the element: */
    *_pElement = _hash->m_hashSet[placeOfElementToRemove];
    _hash->m_hashSet[placeOfElementToRemove] = _hash->m_dummyKey;
    _hash->m_numOfElementsInHashSet--;

    return HASHSET_SUCCESS;
}


int HashSetIsFoundKey(const HashSet* _hash, void* _key)
{
    size_t hashSetIndex;

    if(!_hash || !_key)
    {
        return HASHSET_UNINITIALIZED;
    }

    hashSetIndex = _hash->m_hashFunction(_key) % _hash->m_hashSetCapacity;

    return (GetPlaceOfElementInHashSet(_hash, _key, hashSetIndex) != MAX_SIZE_T) ? KEY_FOUND : KEY_NOT_FOUND;
}


size_t HashSetGetNumOfElements(const HashSet* _hash)
{
    return _hash->m_numOfElementsInHashSet;
}


size_t HashSetGetCapacity(const HashSet* _hash)
{
    return _hash->m_hashSetOriginalSize;
}


double HashSetGetAverageAmountOfReHashOperations(const HashSet* _hash)
{
    return ((double)_hash->m_totalCountOfReHashOperations / _hash->m_totalCountOfInsertions);
}


size_t HashSetGetTotalCountOfReHashOperations(const HashSet* _hash)
{
    return _hash->m_totalCountOfReHashOperations;
}


/* --------------------------------------- End of Main API Functions ---------------------------------- */


/* Helper Function: */

static size_t RefactorHashSetSize(size_t _size)
{
    return (size_t)((double)_size * REFACTORING_RATE);
}


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


static HashSet* InitializeNewHashSet(HashSet* _hash, size_t _sizeOfHashSet, HashFunction _hashFunction, ReHashFunction _reHashFunction, void* _reHashFunctionContext, EqualityFunction _equalityFunction)
{
    size_t refactoredSize = RefactorHashSetSize(_sizeOfHashSet);
    refactoredSize = FindNearestPrimeNumber(refactoredSize);

    _hash->m_hashSet = calloc(refactoredSize, sizeof(void*)); /* Put NULLs on empty places */
    _hash->m_dummyKey = malloc(SINGLE_BYTE);
    if(!_hash->m_hashSet || !_hash->m_dummyKey)
    {
        if(_hash->m_dummyKey)
        {
            free(_hash->m_dummyKey);
        }

        if(_hash->m_hashSet)
        {
            free(_hash->m_hashSet);
        }

        free(_hash);

        return NULL;
    }

    _hash->m_hashSetOriginalSize = _sizeOfHashSet;
    _hash->m_hashSetCapacity = refactoredSize;
    _hash->m_maxAmountOfReHashOperationsInARow = 0;
    _hash->m_numOfElementsInHashSet = 0;

    _hash->m_hashFunction = _hashFunction;
    _hash->m_equalityFunction = _equalityFunction;

    if(_reHashFunction) /* ReHash function was given by the user */
    {
        _hash->m_reHashFunction = _reHashFunction;
        _hash->m_reHashFunctionContext = _reHashFunctionContext;
    }
    else /* No ReHash function was given -> using default ReHash function */
    {
        _hash->m_reHashFunction = &DefaultReHashFunction;
        _hash->m_reHashFunctionContext = (void*)&_hash->m_hashSetCapacity;
    }

    /* Statistic parameters: */
    _hash->m_totalCountOfReHashOperations = 0;
    _hash->m_totalCountOfInsertions = 0;

    return _hash;
}


static size_t DefaultReHashFunction(size_t _hashSetIndex, void* _hashSetCapacity)
{
    return (_hashSetIndex + 1) % (*((size_t*)_hashSetCapacity));
}


static int IsKeyExist(void* _hashSetElement, void* _newElement, EqualityFunction _equalityFunction)
{
    return _equalityFunction(_hashSetElement, _newElement);
}


static size_t GetPlaceOfElementInHashSet(const HashSet* _hash, void* _element, size_t _hashSetIndex)
{
    size_t i;

    for(i = 0; i < _hash->m_maxAmountOfReHashOperationsInARow; i++)
    {
        if(!(_hash->m_hashSet[_hashSetIndex])) /* Empty place - cannot be */
        {
            return MAX_SIZE_T; /* On error - not found */
        }
        else if(_hash->m_hashSet[_hashSetIndex] == _hash->m_dummyKey) /* Element was removed from here - can be */
        {
            continue;
        }

        if(IsKeyExist(_hash->m_hashSet[_hashSetIndex], _element, _hash->m_equalityFunction))
        {
            return _hashSetIndex;
        }

        _hashSetIndex = _hash->m_reHashFunction(_hashSetIndex, _hash->m_reHashFunctionContext); /* Used to get the next correct place */
    }

    /* If got to here - element does not exists */
    return MAX_SIZE_T; /* On error - not found */
}
