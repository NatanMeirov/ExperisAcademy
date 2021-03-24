/**
 * @file Heap.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Maximum int Heap Data Structure source file
 * @version 0.1
 * @date 2021-03-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/* Includes: */

#include <stdio.h> /* printf */
#include <stdlib.h> /* NULL, size_t, malloc, free */
#include "Vector.h"
#include "Heap.h"
#include "ADTDefs.h"

/* Defines and Macros: */

#define MAGIC_NUM 2286491
#define PARENT(x) (((x) - 1) / 2)
#define LEFT_SON(x) ((2 * (x)) + 1)
#define RIGHT_SON(x) ((2 * (x)) + 2)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

struct Heap
{
    Vector* m_vector;
    size_t m_heapSize;
    int m_magicNumber;
};


/* Validation Functions Declarations: */

static ADTErr ValidateNotNullHeap(Heap* _heap);
static ADTErr ValidateNotNullVector(Vector* _vector);
static ADTErr ValidateNotNullItem(int* _item);


/* Helper Functions Declarations: */

static void InitializeNewHeap(Heap* _heap, Vector* _vector);
static void BuildHeapFromVectorInPlace(Heap* _heap);
static void Heapify(Heap* _heap, size_t _currentIndex);
static size_t GetLargestItemIndexAndGetItemByRef(Heap* _heap, size_t _currentIndex, int* _largestItem);
static void ShiftUp(Heap* _heap, size_t _currentIndex);


/*------------------------------------ Main API Functions: ----------------------------------------*/

Heap* HeapBuild(Vector* _vector)
{
    Heap* newHeap = NULL;

    if(ValidateNotNullVector(_vector) == ERR_NOT_INITIALIZED)
    {
        return NULL;
    }

    newHeap = malloc(sizeof(Heap));

    if(ValidateNotNullHeap(newHeap) == ERR_NOT_INITIALIZED)
    {
        return NULL;
    }

    InitializeNewHeap(newHeap, _vector);

    BuildHeapFromVectorInPlace(newHeap);

    return newHeap;
}


void HeapDestroy(Heap* _heap)
{
    if(ValidateNotNullHeap(_heap) != ERR_NOT_INITIALIZED && _heap->m_magicNumber == MAGIC_NUM)
    {
        _heap->m_magicNumber = 0;
        _heap->m_vector = NULL;
        free(_heap);
    }
}


ADTErr HeapInsert(Heap* _heap, int _data)
{
    ADTErr statusCode;

    if((statusCode = ValidateNotNullHeap(_heap)) != ERR_OK || (statusCode = ValidateNotNullVector(_heap->m_vector)) != ERR_OK)
    {
        return statusCode;
    }

    if((statusCode = VectorAdd(_heap->m_vector, _data)) != ERR_OK)
    {
        return statusCode;
    }

    _heap->m_heapSize++;
    ShiftUp(_heap, _heap->m_heapSize - 1);

    return ERR_OK;
}


ADTErr HeapMax(Heap* _heap, int* _data)
{
    ADTErr statusCode;

    if((statusCode = ValidateNotNullHeap(_heap)) != ERR_OK || (statusCode = ValidateNotNullVector(_heap->m_vector)) != ERR_OK
    || (statusCode = ValidateNotNullItem(_data)) != ERR_OK)
    {
        return statusCode;
    }

    if(_heap->m_heapSize == 0)
    {
        return ERR_UNDERFLOW;
    }

    VectorGet(_heap->m_vector, 0, _data);

    return ERR_OK;
}


ADTErr HeapExtractMax(Heap* _heap, int* _data)
{
    ADTErr statusCode;
    int newTopOfHeap;

    if((statusCode = ValidateNotNullHeap(_heap)) != ERR_OK || (statusCode = ValidateNotNullVector(_heap->m_vector)) != ERR_OK
    || (statusCode = ValidateNotNullItem(_data)) != ERR_OK)
    {
        return statusCode;
    }

    if(_heap->m_heapSize == 0)
    {
        return ERR_UNDERFLOW;
    }

    /* Extracting the heap's max, and moving the last leaf of the heap to the root - to Heapify it from the top */
    VectorGet(_heap->m_vector, 0, _data);
    _heap->m_heapSize--;
    VectorDelete(_heap->m_vector, &newTopOfHeap);
    VectorSet(_heap->m_vector, 0, newTopOfHeap);

    Heapify(_heap, 0);

    return ERR_OK;
}


size_t HeapGetItemsCount(Heap* _heap)
{
    if(ValidateNotNullHeap(_heap) != ERR_OK || ValidateNotNullVector(_heap->m_vector) != ERR_OK)
    {
        return 0;
    }

    return _heap->m_heapSize;
}


void HeapPrint(Heap* _heap)
{
    if(ValidateNotNullHeap(_heap) != ERR_OK || ValidateNotNullVector(_heap->m_vector) != ERR_OK)
    {
        return;
    }

    VectorPrint(_heap->m_vector);
}


/*---------------------------------- End of Main API Functions ----------------------------------------*/


/* Validation Functions: */

static ADTErr ValidateNotNullHeap(Heap* _heap)
{
    return (!_heap) ? ERR_NOT_INITIALIZED : ERR_OK;
}


static ADTErr ValidateNotNullVector(Vector* _vector)
{
    return (!_vector) ? ERR_NOT_INITIALIZED : ERR_OK;
}


static ADTErr ValidateNotNullItem(int* _item)
{
    return (!_item) ? ERR_NOT_INITIALIZED : ERR_OK;
}


/* Helper Functions: */

static void InitializeNewHeap(Heap* _heap, Vector* _vector)
{
    size_t vectorItemsCount;
    VectorGetItemsNumber(_vector, &vectorItemsCount);

    _heap->m_vector = _vector;
    _heap->m_heapSize = vectorItemsCount;
    _heap->m_magicNumber = MAGIC_NUM;
}


static void BuildHeapFromVectorInPlace(Heap* _heap)
{
    size_t i, youngestParent = PARENT(_heap->m_heapSize - 1); /* The last father in the heap (all items after it are leaves) */

    for(i = youngestParent; i > 0; i--)
    {
        Heapify(_heap, i);
    }

    Heapify(_heap, 0); /* i is size_t - can't be negative (must add this line too, to trigger Heapify for index = 0) */
}


static void Heapify(Heap* _heap, size_t _currentIndex)
{
    size_t largestItemIndex;
    int currentItem, largestItem;

    if(LEFT_SON(_currentIndex) > _heap->m_heapSize - 1)
    {
        return; /* Current item will be a leaf - No need to Heapify further */
    }

    VectorGet(_heap->m_vector, _currentIndex, &currentItem);

    largestItem = currentItem; /* For now, current item is the largest */
    largestItemIndex = GetLargestItemIndexAndGetItemByRef(_heap, _currentIndex, &largestItem);

    if(largestItemIndex == _currentIndex)
    {
        return; /* Finished heapifing the current item */
    }

    /* Swaping: */
    VectorSet(_heap->m_vector, _currentIndex, largestItem);
    VectorSet(_heap->m_vector, largestItemIndex, currentItem);

    Heapify(_heap, largestItemIndex); /* Continue heapify the current item */
}


static size_t GetLargestItemIndexAndGetItemByRef(Heap* _heap, size_t _currentIndex, int* _largestItem)
{
    size_t largestItemIndex, leftIndex = LEFT_SON(_currentIndex), rightIndex = RIGHT_SON(_currentIndex);
    int leftItem, rightItem;
    int currentItemValue = *_largestItem; /* Saving current item's value */

    VectorGet(_heap->m_vector, leftIndex, &leftItem);

    *_largestItem = MAX(*_largestItem, leftItem);

    if(rightIndex <= _heap->m_heapSize - 1) /* Not always will be a right son to a parent */
    {
        VectorGet(_heap->m_vector, rightIndex, &rightItem);
        *_largestItem = MAX(*_largestItem, rightItem);
    }

    /* In this stage, largest item is stored in the correct address location (passed by reference) */
    if(*_largestItem == currentItemValue) /* Did not change */
    {
        largestItemIndex = _currentIndex;
    }
    else if(*_largestItem == leftItem)
    {
        largestItemIndex = leftIndex;
    }
    else /* largestItem == rightItem [&& rightIndex <= _heap->m_heapSize - 1 (Must be - if largest is not current, and is not left... must be right and the right index must be valid in that case)] */
    {
        largestItemIndex = rightIndex;
    }

    return largestItemIndex;
}


static void ShiftUp(Heap* _heap, size_t _currentIndex)
{
    size_t parentIndex;
    int currentItem, parentItem;

    if(_currentIndex == 0)
    {
        return; /* The current item is the new root */
    }

    parentIndex = PARENT(_currentIndex);

    VectorGet(_heap->m_vector, _currentIndex, &currentItem);
    VectorGet(_heap->m_vector, parentIndex, &parentItem);

    if(currentItem < parentItem)
    {
        return; /* Finished shifting up the current item */
    }

    /* Swaping: */
    VectorSet(_heap->m_vector, parentIndex, currentItem);
    VectorSet(_heap->m_vector, _currentIndex, parentItem);

    ShiftUp(_heap, parentIndex); /* Continue shifting up the current item */
}