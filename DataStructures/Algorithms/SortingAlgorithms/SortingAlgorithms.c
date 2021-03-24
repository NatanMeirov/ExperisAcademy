/**
 * @file SortingAlgorithms.c
 * @author Natan Meirov (NatanMeirov@dgmail.com)
 * @brief A Sorting Algorithms (for int Vector Data Structure) header file
 * @version 0.1
 * @date 2021-03-04
 *
 * @copyright Copyright (c) 2021
 *
 */

/* Includes: */

#include <stdlib.h> /* size_t, malloc, calloc, free */
#include "SortingAlgorithms.h"
#include "ADTDefs.h"
#include "../../Dynamic_Vector/Vector.h"
#include "../../Stack/Stack.h"

#define SIZE 10
#define DIGITS_COUNT 10

/* Helper Functions Declarations: */

/* Setting places for first and second item by arguments order */
static void SetItemsInVectorByOrder(Vector* _vector, size_t _firstItemIndex, size_t _secondItemIndex, int _itemToSetInFirstPlace, int _itemToSetInSecondPlace);

static void SwapItemsInVector(Vector* _vector, size_t _firstItemIndex, size_t _secondItemIndex);

/* Single pass (forward mode or reversed mode) of sorting for a Vector (In-Place) */
static int OnePassSortInRange(Vector* _vector, size_t _startingIndex, size_t _endingIndex, int isReversedMode);

static int CheckForwardDirectionCondition(size_t _startIndex, size_t _endIndex);
static int CheckReverseDirectionCondition(size_t _startIndex, size_t _endIndex);

static void QuickSortRec(Vector* _vector, size_t _begin, size_t _end);
static size_t Partition(Vector* _vector, size_t _begin, size_t _end);
static size_t GetMedianIndexFromThreeItemsInVector(Vector* _vector, size_t _startingIndex, size_t _endingIndex);
static int IsLessThenOrEqualToPivot(Vector* _vector, int _pivot, size_t _indexOfItemToCheck);
static int IsGreaterThenPivot(Vector* _vector, int _pivot, size_t _indexOfItemToCheck);

static void QuickSortIter(Vector* _vector, size_t _begin, size_t _end);

static size_t FindMinimumItemIndexInVector(Vector* _vector, size_t _startIndex, size_t _endIndex);

static void GapJumpsInsertionSort(Vector* _vector, size_t _vectorSize, size_t _gap);
static void PutItemInCorrectPlaceWithGapJumps(Vector* _vector, size_t _index, size_t _gap);

static void MergeSortRec(Vector* _vector, size_t _begin, size_t _end, int* _workingArray);
static void Merge(Vector* _vector, size_t _begin, size_t _middle, size_t _end, int* _workingArray);
static void CopyItemsFromArrayToVector(Vector* _vector, size_t _startingIndex, size_t _endingIndex, int* _workingArray);

static void InitializeCountingArray(Vector* _vector, size_t _sizeOfVector, int* _countingArray, int _minValue);
static void NotStableCountingArrayCopy(Vector* _vector, int* _countingArray, int _minValue, int _maxValue);
static void AccumulateCountingArray(int* _countingArray, size_t _countingArraySize);
static void SortVectorUsingCountingArray(Vector* _vector, size_t _sizeOfVector, int* _countingArray, int _minValue, int* _workingArray);

static void SortVectorBySpecificDigit(Vector* _vector, size_t _currentDigitFromLeft);
static void RadixCountingSort(Vector* _vector, size_t _normalizationNumber);


/*-------------------------------- Main API Functions: -----------------------------------------*/


ADTErr BubbleSort(Vector* _vector)
{
    size_t sizeOfVector;
    int firstItem, secondItem;
    int vectorChangedFlag = 0; /* Boolean flag */
    register size_t i, j, totalIterations = 0;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

	for(i = 0; i < sizeOfVector; i++)
	{
		for(j = 0; j < sizeOfVector - 1 - totalIterations; j++)
		{
            VectorGet(_vector, j, &firstItem);
            VectorGet(_vector, j + 1, &secondItem);

			if(firstItem > secondItem)
			{
                /* Swapping the items by setting them in crossed order */
				SetItemsInVectorByOrder(_vector, j, j + 1, secondItem, firstItem);
				vectorChangedFlag = 1;
			}
		}

		if(!vectorChangedFlag)
		{
		/* Vector already sorted */
			break;
		}

		vectorChangedFlag = 0;
		totalIterations++;
	}

    return ERR_OK;
}


ADTErr ShakeSort(Vector* _vector)
{
    size_t sizeOfVector, iterations = 0;
    int vectorChangedFlag = 0; /* Boolean flag */
    register size_t start = 0, end;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    end = sizeOfVector - 1;

    while(start < end)
    {
        vectorChangedFlag = OnePassSortInRange(_vector, start, end, 0);
        if(!vectorChangedFlag)
        {
            /* Vector already sorted */
            break;
        }
        end--;

        vectorChangedFlag = OnePassSortInRange(_vector, end, start, 1);
        if(!vectorChangedFlag)
        {
            /* Vector already sorted */
            break;
        }
        start++;


        vectorChangedFlag = 0;
    }

    return ERR_OK;
}


ADTErr QuickSort(Vector* _vector)
{
    size_t sizeOfVector;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    QuickSortRec(_vector, 0, sizeOfVector - 1);

    return ERR_OK;
}


ADTErr SelectionSort(Vector* _vector)
{
    size_t minItemIndex, sizeOfVector, sortedPartIndex;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    for(sortedPartIndex = 0; sortedPartIndex < sizeOfVector - 1; sortedPartIndex++)
    {
        minItemIndex = FindMinimumItemIndexInVector(_vector, sortedPartIndex, sizeOfVector - 1);
        SwapItemsInVector(_vector, sortedPartIndex, minItemIndex);
    }

    return ERR_OK;
}


ADTErr InsertionSort(Vector* _vector)
{
    size_t sizeOfVector;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    GapJumpsInsertionSort(_vector, sizeOfVector, 1);

    return ERR_OK;
}


ADTErr ShellSort(Vector* _vector)
{
    size_t minItemIndex, sizeOfVector, gap;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    gap = sizeOfVector / 2; /* Initialized gap */
    while(gap > 0)
    {
        GapJumpsInsertionSort(_vector, sizeOfVector, gap);
        gap /= 2;
    }

    return ERR_OK;
}


ADTErr MergeSort(Vector* _vector)
{
    size_t sizeOfVector, begin = 0, end;
    int* workingArray = NULL;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    workingArray = malloc(sizeOfVector * sizeof(int));

    if(!workingArray)
    {
        return ERR_ALLOCATION_FAILED;
    }

    end = sizeOfVector - 1;

    MergeSortRec(_vector, begin, end, workingArray);

    free(workingArray);

    return ERR_OK;
}


ADTErr CountingSort(Vector* _vector, int _minValue, int _maxValue, int isStable)
{
    size_t sizeOfVector;
    int* countingArray = NULL;
    int* workingArray = NULL;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);
    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    countingArray = calloc((_maxValue - _minValue), sizeof(int)); /* Cleaned array initialized with zeroes */
    if(!countingArray)
    {
        return ERR_ALLOCATION_FAILED;
    }

    InitializeCountingArray(_vector, sizeOfVector, countingArray, _minValue);

    if(!isStable)
    {
        NotStableCountingArrayCopy(_vector, countingArray, _minValue, _maxValue);
        return ERR_OK;
    }

    /* Steps for Stable Counting Sort: */

    AccumulateCountingArray(countingArray, (_maxValue - _minValue));

    /* Lazy initialization of workingArray: */
    workingArray = malloc(sizeOfVector * sizeof(int));
    if(!workingArray)
    {
        return ERR_ALLOCATION_FAILED;
    }

    SortVectorUsingCountingArray(_vector, sizeOfVector, countingArray, _minValue, workingArray);

    return ERR_OK;
}


ADTErr RadixSort(Vector* _vector, size_t _numberOfDigits)
{
    size_t i, sizeOfVector, temp;

    if(!_vector)
    {
        return ERR_NOT_INITIALIZED;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);

    if(sizeOfVector < 2)
    {
        return ERR_OK; /* No need to sort */
    }

    temp = _numberOfDigits;

    for(i = 1; i <= _numberOfDigits; i++)
    {
        SortVectorBySpecificDigit(_vector, i);
    }

    return ERR_OK;
}

/*----------------------------- End of Main API Functions ---------------------------------------*/


/* Helper Functions: */

static void SetItemsInVectorByOrder(Vector* _vector, size_t _firstItemIndex, size_t _secondItemIndex, int _itemToSetInFirstPlace, int _itemToSetInSecondPlace)
{
	VectorSet(_vector, _firstItemIndex, _itemToSetInFirstPlace);
    VectorSet(_vector, _secondItemIndex, _itemToSetInSecondPlace);
}

static void SwapItemsInVector(Vector* _vector, size_t _firstItemIndex, size_t _secondItemIndex)
{
    int firstItem, secondItem;
    VectorGet(_vector, _firstItemIndex, &firstItem);
    VectorGet(_vector, _secondItemIndex, &secondItem);
    SetItemsInVectorByOrder(_vector, _firstItemIndex, _secondItemIndex, secondItem, firstItem);
}

static int OnePassSortInRange(Vector* _vector, size_t _startingIndex, size_t _endingIndex, int isReversedMode)
{
    size_t i;
    int indexOffset, iterationStep, firstItem, secondItem, vectorChangedFlag = 0;
    /**
     * @brief A pointer to a function to trigger a condition checking, depending on the swapping direction mode
     */
    int (*ConditionCheckFunction)(size_t _firstIndex, size_t _secondIndex);

    if(isReversedMode)
    {
        iterationStep = -1;
        ConditionCheckFunction = &CheckReverseDirectionCondition;
        indexOffset = -1;
    }
    else
    {
        iterationStep = 1;
        ConditionCheckFunction = &CheckForwardDirectionCondition;
        indexOffset = 0;
    }

    for(i = _startingIndex; (*ConditionCheckFunction)(i, _endingIndex); i += iterationStep)
    {
        VectorGet(_vector, i + indexOffset, &firstItem);
        VectorGet(_vector, i + 1 + indexOffset, &secondItem);

        if(firstItem > secondItem)
		{
            /* Swapping the items by setting them in crossed order */
			SetItemsInVectorByOrder(_vector, i + indexOffset, i + 1 + indexOffset, secondItem, firstItem);
			vectorChangedFlag = 1;
		}
    }

    return vectorChangedFlag;
}


static int CheckForwardDirectionCondition(size_t _startIndex, size_t _endIndex)
{
    return (_startIndex < _endIndex)? 1 : 0;
}


static int CheckReverseDirectionCondition(size_t _startIndex, size_t _endIndex)
{
    return (_startIndex > _endIndex)? 1 : 0;
}


static void QuickSortRec(Vector* _vector, size_t _begin, size_t _end)
{
    size_t partitionIndex;

    if(_begin >= _end)
    {
        return; /* One last item - Vector is already sorted */
    }

    partitionIndex = Partition(_vector, _begin, _end);

    if(partitionIndex > 0) /* because of size_t */
    {
        QuickSortRec(_vector, _begin, partitionIndex - 1);
    }
    QuickSortRec(_vector, partitionIndex + 1, _end);
}


static size_t Partition(Vector* _vector, size_t _begin, size_t _end)
{
    int pivot, firstItem, secondItem;
    size_t findLessThenPivotIndex = _end, findGreaterThenPivotIndex = _begin + 1;
    size_t pivotIndex = GetMedianIndexFromThreeItemsInVector(_vector, _begin, _end);

    /* Place pivot in the first index place: */
    SwapItemsInVector(_vector, _begin, pivotIndex);
    VectorGet(_vector, _begin , &pivot);

    while(findGreaterThenPivotIndex <= findLessThenPivotIndex)
    {
        while(findGreaterThenPivotIndex <= _end && IsLessThenOrEqualToPivot(_vector, pivot, findGreaterThenPivotIndex))
        {
            findGreaterThenPivotIndex++;
        }

        while(IsGreaterThenPivot(_vector, pivot, findLessThenPivotIndex)) /* && _begin < findLessThenPivotIndex - NO need to check, because the Pivot is in the first place, and the condition will (is greater then...) always will stop at least in the Pivot (it is the  limit)*/
        {
            findLessThenPivotIndex--;
        }

        if(findGreaterThenPivotIndex < findLessThenPivotIndex)
        {
            SwapItemsInVector(_vector, findGreaterThenPivotIndex, findLessThenPivotIndex);
        }
    }

    SwapItemsInVector(_vector, _begin, findLessThenPivotIndex);

    return findLessThenPivotIndex; /* The corrent place (index) of the pivot in the sorted Vector */
}


static size_t GetMedianIndexFromThreeItemsInVector(Vector* _vector, size_t _startingIndex, size_t _endingIndex)
{
	size_t middleIndex = (_startingIndex + _endingIndex) / 2;
	int startItem, middleItem, endItem, median, x, y, z;

	VectorGet(_vector, _startingIndex, &startItem);
	VectorGet(_vector, middleIndex, &middleItem);
	VectorGet(_vector, _endingIndex, &endItem);

	x = startItem - middleItem;
	y = middleItem - endItem;
	z = startItem - endItem;

	median = ((x * y) > 0) ? middleItem : (((x * z) > 0) ? endItem : startItem);

	return (median == startItem) ? _startingIndex : ((median == endItem) ? _endingIndex : middleIndex);
}


static int IsLessThenOrEqualToPivot(Vector* _vector, int _pivot, size_t _indexOfItemToCheck)
{
    int item;
    VectorGet(_vector, _indexOfItemToCheck, &item);

    return (_pivot >= item) ? 1 : 0;
}


static int IsGreaterThenPivot(Vector* _vector, int _pivot, size_t _indexOfItemToCheck)
{
    int item;
    VectorGet(_vector, _indexOfItemToCheck, &item);

    return (_pivot < item) ? 1 : 0;
}


static void QuickSortIter(Vector* _vector, size_t _begin, size_t _end)
{
    size_t partitionIndex, start, end;
    Stack* stack = StackCreate(SIZE, SIZE);

    if(!stack)
    {
        return;
    }

    if(StackPush(stack, (int)_begin) != ERR_OK || StackPush(stack, (int)_end) != ERR_OK)
    {
        return;
    }

    while(!StackIsEmpty(stack))
    {
        StackPop(stack, (int*)&start);
        StackPop(stack, (int*)&end);

        if(start == end)
        {
            continue;
        }

        partitionIndex = Partition(_vector, start, end);

        if(partitionIndex > start)
        {
            if(StackPush(stack, (int)start) != ERR_OK || StackPush(stack, (int)partitionIndex - 1) != ERR_OK)
            {
                return;
            }
        }

        if(partitionIndex < end)
        {
            if(StackPush(stack, (int)partitionIndex + 1) != ERR_OK || StackPush(stack, (int)end) != ERR_OK)
            {
                return;
            }
        }
    }

    StackDestroy(stack);
}


static size_t FindMinimumItemIndexInVector(Vector* _vector, size_t _startIndex, size_t _endIndex)
{
    size_t i, currentMinIndex = _startIndex;
    int minItem, tempItem;

    VectorGet(_vector, _startIndex, &minItem);

    for(i = _startIndex + 1; i <= _endIndex; i++)
    {
        VectorGet(_vector, i, &tempItem);

        if(minItem > tempItem)
        {
            minItem = tempItem;
            currentMinIndex = i;
        }
    }

    return currentMinIndex;
}


static void GapJumpsInsertionSort(Vector* _vector, size_t _vectorSize, size_t _gap)
{
    size_t sortedPartIndex;
    int currentItem;

    for(sortedPartIndex = _gap; sortedPartIndex < _vectorSize; sortedPartIndex++)
    {
        PutItemInCorrectPlaceWithGapJumps(_vector, sortedPartIndex, _gap);
    }
}


static void PutItemInCorrectPlaceWithGapJumps(Vector* _vector, size_t _index, size_t _gap)
{
    size_t i;
    int currentItem, tempItem;

    VectorGet(_vector, _index, &currentItem);

    for (i = _index; i >= _gap; i -= _gap)
    {
        VectorGet(_vector, i - _gap, &tempItem);

        if(currentItem < tempItem)
			{
            /* Swapping the items by setting them in crossed order */
			SetItemsInVectorByOrder(_vector, i, i - _gap, tempItem, currentItem);
        }
        else
        {
            break; /* The current place is the correct place for the item */
        }

    }
}


static void MergeSortRec(Vector* _vector, size_t _begin, size_t _end, int* _workingArray)
{
    size_t middle;

    if(_begin >= _end) /* One item only */
    {
        return;
    }

    middle = (_begin + _end) / 2;

    MergeSortRec(_vector, _begin, middle, _workingArray);
    MergeSortRec(_vector, middle + 1, _end, _workingArray);

    Merge(_vector, _begin, middle, _end, _workingArray);
}


static void Merge(Vector* _vector, size_t _begin, size_t _middle, size_t _end, int* _workingArray)
{
    size_t i = _begin, j = _middle + 1, workingArrayIndex = _begin;
    int firstItem, secondItem;

    VectorGet(_vector, i, &firstItem);
    VectorGet(_vector, j, &secondItem);

    while(i <= _middle && j <= _end)
    {
        if(firstItem < secondItem)
        {
            _workingArray[workingArrayIndex++] = firstItem;
            i++;
            VectorGet(_vector, i, &firstItem);
        }
        else
        {
            _workingArray[workingArrayIndex++] = secondItem;
            j++;
            VectorGet(_vector, j, &secondItem);
        }
    }

    /* Fill array with tail: */
    if(i == _middle + 1)
    {
        while(j <= _end)
        {
            VectorGet(_vector, j, &secondItem);
            _workingArray[workingArrayIndex++] = secondItem;
            j++;
        }
    }
    else
    {
        while(i <= _middle)
        {
            VectorGet(_vector, i, &firstItem);
            _workingArray[workingArrayIndex++] = firstItem;
            i++;
        }
    }

    /* Copy items from working array to the Vector: */
    CopyItemsFromArrayToVector(_vector, _begin, _end, _workingArray);
}

static void CopyItemsFromArrayToVector(Vector* _vector, size_t _startingIndex, size_t _endingIndex, int* _workingArray)
{
    size_t i;

    for(i = _startingIndex; i <= _endingIndex; i++)
    {
        VectorSet(_vector, i, _workingArray[i]);
    }
}


static void InitializeCountingArray(Vector* _vector, size_t _sizeOfVector, int* _countingArray, int _minValue)
{
    size_t i;
    int item;

    for(i = 0; i < _sizeOfVector; i++)
    {
        VectorGet(_vector, i, &item);
        _countingArray[item - _minValue]++;
    }
}


static void NotStableCountingArrayCopy(Vector* _vector, int* _countingArray, int _minValue, int _maxValue)
{
    size_t i, j, counter, vectorIndex = 0;
    int item;

    for(i = 0; i < (_maxValue - _minValue); i++)
    {
        if(_countingArray[i]) /* If not 0 */
        {
            counter = _countingArray[i];
            item = i + _minValue;
            for(j = 0; j < counter; j++)
            {
                VectorSet(_vector, vectorIndex, item);
                vectorIndex++;
            }
        }
    }
}


static void AccumulateCountingArray(int* _countingArray, size_t _countingArraySize)
{
    size_t i;

    for(i = 1; i < _countingArraySize; i++)
    {
        _countingArray[i] += _countingArray[i - 1];
    }
}


static void SortVectorUsingCountingArray(Vector* _vector, size_t _sizeOfVector, int* _countingArray, int _minValue, int* _workingArray)
{
    size_t i;
    int item;

    for(i = _sizeOfVector; i > 0; i--)
    {
        VectorGet(_vector, i - 1, &item);
        _workingArray[_countingArray[item - _minValue] - 1] = item; /* -1 to get to the right index place */
        _countingArray[item - _minValue]--;
    }

    /* Copy items from working array to the Vector: */
    CopyItemsFromArrayToVector(_vector, 0, _sizeOfVector - 1, _workingArray);
}


static void SortVectorBySpecificDigit(Vector* _vector, size_t _currentDigitFromLeft)
{
    size_t i, normalizationNumber = 1;

    for(i = 2; i <= _currentDigitFromLeft; i++)
    {
        normalizationNumber *= 10;
    }

    RadixCountingSort(_vector, normalizationNumber);
}

/* Can easily implement this function in generic way - changing the above CountingSort to normalize the numbers with pointer to function (to trigger on each item to normalize it (and the regular just use a NULL - no func call - no normalization))*/
static void RadixCountingSort(Vector* _vector, size_t _normalizationNumber)
{
    int item;
    size_t i, sizeOfVector;
    int* countingArray = calloc(DIGITS_COUNT, sizeof(int));
    int* workingArray = NULL;

    if(!countingArray)
    {
        return;
    }

    VectorGetItemsNumber(_vector, &sizeOfVector);
    workingArray = malloc(sizeOfVector * sizeof(int));
    if(!workingArray)
    {
        return;
    }

    /* Initialize Counting Array: */
    for(i = 0; i < sizeOfVector; i++)
    {
        VectorGet(_vector, i, &item);
        countingArray[((item / _normalizationNumber) % 10)]++;
    }

    AccumulateCountingArray(countingArray, DIGITS_COUNT);

    for(i = sizeOfVector; i > 0; i--)
    {
        VectorGet(_vector, i - 1, &item);
        workingArray[countingArray[((item / _normalizationNumber) % 10)] - 1] = item; /* -1 to get to the right index place */
        countingArray[((item / _normalizationNumber) % 10)]--;
    }

    /* Copy items from working array to the Vector: */
    CopyItemsFromArrayToVector(_vector, 0, sizeOfVector - 1, workingArray);

    free(countingArray);
    free(workingArray);
}