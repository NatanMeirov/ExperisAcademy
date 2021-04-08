/**
 * @file GenericHeap.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief Creates a generic data type Binary Heap, over a generic data type Vector
 * In version 2.0: Performance improvements
 * @version 2.0
 * @date 2021-03-18
 *
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* malloc, free */
#include "../../Include/GenericHeap/GenericHeap.h"
#include "../../Include/GenericVector/GenericVector.h"


/* Defines and Macros: */

#define PARENT(x) (((x) - 1) / 2)
#define LEFT_SON(x) ((2 * (x)) + 1)
#define RIGHT_SON(x) ((2 * (x)) + 2)
#define MAX_SIZE_T ((size_t) -1) /* -1 in size_t - is the maximum size_t value */

struct Heap
{
    Vector* m_vector;
    size_t m_heapSize;
    LessThanComparator m_ptrFuncLessThen;
};


/* Validation Functions Declarations: */

static HeapResult MapVectorErrorToHeapError(VectorResult _vecError);


/* Helper Functions Declarations: */

static void InitializeNewHeap(Heap* _heap, Vector* _vector, LessThanComparator _ptrFuncLessThen);
static void BuildHeapFromVectorInPlace(Heap* _heap);
static void Heapify(Heap* _heap, size_t _currentIndex);
static size_t GetLargestItemIndexAndGetItemByRef(Heap* _heap, size_t _currentIndex, void** _largestItem);
static void ShiftUp(Heap* _heap, size_t _currentIndex);


/*------------------------------------ Main API Functions ----------------------------------------*/

Heap* HeapBuild(Vector* _vector, LessThanComparator _ptrFuncLessThen)
{
    Heap* newHeap = NULL;

    if(!_vector || !_ptrFuncLessThen)
    {
        return NULL;
    }

    newHeap = malloc(sizeof(Heap));
    if(!newHeap)
    {
        return NULL;
    }

    InitializeNewHeap(newHeap, _vector, _ptrFuncLessThen);

    BuildHeapFromVectorInPlace(newHeap);

    return newHeap;
}


Vector* HeapDestroy(Heap** _heap)
{
    Vector* vector = NULL;

    if(!_heap || !(*_heap) || !((*_heap)->m_vector))
    {
        return NULL;
    }

    vector = (*_heap)->m_vector;
    free(*_heap);
    *_heap = NULL;

    return vector;
}


HeapResult HeapInsert(Heap* _heap, void* _element)
{
    VectorResult vecStatusCode;

    if(!_heap || !(_heap->m_vector) || !_element /* Heap cannot include NULLs */)
    {
        return HEAP_NOT_INITIALIZED;
    }

    if((vecStatusCode = VectorAppend(_heap->m_vector, _element)) != VECTOR_SUCCESS)
    {
        return MapVectorErrorToHeapError(vecStatusCode);
    }

    _heap->m_heapSize++;
    ShiftUp(_heap, _heap->m_heapSize - 1);

    return HEAP_SUCCESS;
}


const void* HeapPeekTop(const Heap* _heap)
{
    void* topElement = NULL;

    if(!_heap || !(_heap->m_vector))
    {
        return NULL;
    }

    if(_heap->m_heapSize == 0)
    {
        return NULL; /* Heap is empty */
    }

    VectorGet(_heap->m_vector, 0, &topElement);

    return topElement;
}


void* HeapExtract(Heap* _heap)
{
    void* newTopElementOfHeap = NULL;
    void* topElement = NULL;

    if(!_heap || !(_heap->m_vector))
    {
        return NULL;
    }

    if(_heap->m_heapSize == 0)
    {
        return NULL; /* Heap is empty */
    }

    /* Extracting the heap's max, and moving the last leaf of the heap to the root - to Heapify it from the top */
    VectorGet(_heap->m_vector, 0, &topElement);
    _heap->m_heapSize--;
    VectorRemove(_heap->m_vector, &newTopElementOfHeap);
    VectorSet(_heap->m_vector, 0, newTopElementOfHeap);

    Heapify(_heap, 0);

    return topElement;
}


size_t HeapSize(const Heap* _heap)
{
    if(!_heap || !(_heap->m_vector))
    {
        return MAX_SIZE_T;
    }

    return _heap->m_heapSize;
}


size_t HeapForEach(const Heap* _heap, ActionFunction _action, void* _context)
{
    size_t i;
    void* element = NULL;

    if(!_heap || !(_heap->m_vector) || !_action)
    {
        return MAX_SIZE_T;
    }

    for(i = 0; i < _heap->m_heapSize; i++)
    {
        VectorGet(_heap->m_vector, i, &element);
        if(_action(element, _context) == 0)
        {
            break;
        }
    }

    return i;
}


/*---------------------------------- End of Main API Functions ----------------------------------------*/


/* Validation Functions: */


static HeapResult MapVectorErrorToHeapError(VectorResult _vecError)
{
    return (_vecError == VECTOR_UNINITIALIZED_ERROR) ? HEAP_NOT_INITIALIZED : ((_vecError == VECTOR_ALLOCATION_ERROR) ? HEAP_REALLOCATION_FAILED : HEAP_OVERFLOW_ERROR);
}


/* Helper Functions: */

static void InitializeNewHeap(Heap* _heap, Vector* _vector, LessThanComparator _ptrFuncLessThen)
{
    _heap->m_vector = _vector;
    _heap->m_heapSize = VectorSize(_vector);
    _heap->m_ptrFuncLessThen = _ptrFuncLessThen;
}


static void BuildHeapFromVectorInPlace(Heap* _heap)
{
    size_t i, youngestParent = PARENT(_heap->m_heapSize - 1); /* The last father in the heap (all items after it are leaves) */

    for(i = youngestParent + 1; i > 0; i--)
    {
        Heapify(_heap, i - 1);
    }
}


static void Heapify(Heap* _heap, size_t _currentIndex)
{
    size_t largestItemIndex;
    void* currentItem;
    void* largestItem;

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


static size_t GetLargestItemIndexAndGetItemByRef(Heap* _heap, size_t _currentIndex, void** _largestItem)
{
    size_t largestItemIndex, leftIndex = LEFT_SON(_currentIndex), rightIndex = RIGHT_SON(_currentIndex);
    void* leftItem;
    void* rightItem;
    void* currentItemValue = *_largestItem; /* Saving current item's value */

    VectorGet(_heap->m_vector, leftIndex, &leftItem);

    *_largestItem = (_heap->m_ptrFuncLessThen(leftItem, *_largestItem)) ? *_largestItem : leftItem; /* If leftItem < *_largestItem ? then *_largestItem, else leftItem */

    if(rightIndex <= _heap->m_heapSize - 1) /* Not always will be a right son to a parent */
    {
        VectorGet(_heap->m_vector, rightIndex, &rightItem);
        *_largestItem = (_heap->m_ptrFuncLessThen(rightItem, *_largestItem)) ? *_largestItem : rightItem; /* If rightItem < *_largestItem ? then *_largestItem, else rightItem */
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
    void* currentItem;
    void* parentItem;

    if(_currentIndex == 0)
    {
        return; /* The current item is the new root */
    }

    parentIndex = PARENT(_currentIndex);

    VectorGet(_heap->m_vector, _currentIndex, &currentItem);
    VectorGet(_heap->m_vector, parentIndex, &parentItem);

    if(_heap->m_ptrFuncLessThen(currentItem, parentItem)) /* If currentItem < parentItem */
    {
        return; /* Finished shifting up the current item */
    }

    /* Swaping: */
    VectorSet(_heap->m_vector, parentIndex, currentItem);
    VectorSet(_heap->m_vector, _currentIndex, parentItem);

    ShiftUp(_heap, parentIndex); /* Continue shifting up the current item */
}

