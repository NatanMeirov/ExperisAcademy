#include <algorithm>
#include <iterator>
#include <numeric> // std::accumulate
#include <vector>
#include <cstddef> // size_t
#include <utility> // std::pair


int CountLenOfListRepresentedAsVector(std::vector<int> &A) {
    size_t curIndex = 0;
    size_t counter = 0;
    bool isEndOfList = false;

    while(!isEndOfList)
    {
        ++counter;

        if(A[curIndex] == -1)
        {
            isEndOfList = true;
        }
        else
        {
            curIndex = A[curIndex];
        }
    }

    return counter;
}


// The greater in A where: A[P] - Average
int FindExtremeNumberIndex(std::vector<int> &A) {
    if(A.empty())
    {
        return -1;
    }

    int average = std::accumulate(A.begin(), A.end(), 0) / A.size();

    auto minMaxPair = std::minmax_element(A.begin(), A.end());

    int minDeviation = std::abs((*minMaxPair.first) - average);
    int maxDeviation = std::abs((*minMaxPair.second) - average);

    return maxDeviation > minDeviation ? std::distance(A.begin(), minMaxPair.second) : std::distance(A.begin(), minMaxPair.first);
}


bool IsSortedAfterAtMostOneSwap(std::vector<int> &A) {
    size_t i = 0;
    size_t arrSize = A.size();
    if(!arrSize)
    {
        return true;
    }

    // Find the first "leap" from the beginning:
    size_t leapIndexFromStart = 0;
    for(; i < arrSize - 1; ++i)
    {
        if(A[i] > A[i + 1])
        {
            leapIndexFromStart = i;
            break;
        }
    }

    // Check if array is already sorted (more efficient to do so - skipping another useless O(n) loop)
    if(i == arrSize - 1) // Reached the end of the loop from above, without breaking...
    {
        return true;
    }


    // Find the correct place (from the ending) of that number, after the swap:
    size_t newPlaceAfterSwap = 0;
    for(i = arrSize; i > 0; --i)
    {
        if(A[i - 1] < A[leapIndexFromStart])
        {
            newPlaceAfterSwap = i - 1;
            break;
        }
    }

    std::swap(A[newPlaceAfterSwap], A[leapIndexFromStart]);

    return std::is_sorted(A.begin(), A.end());
}


int FindBeginOfGreatestAccendingSlice(std::vector<int> &A) {
    size_t curSliceStart = 0;
    size_t greatestSliceStart = 0;
    size_t maxSliceSize = 0;
    size_t curSliceCounter = 0;

    size_t arrSize = A.size();
    if(!arrSize)
    {
        return 0;
    }

    for(size_t i = 0; i < arrSize - 1; ++i)
    {
        ++curSliceCounter;
        if(A[i] >= A[i + 1]) // Current slice's ending
        {
            if(curSliceCounter > maxSliceSize)
            {
                greatestSliceStart = curSliceStart;
                maxSliceSize = curSliceCounter;
            }

            curSliceStart = i + 1;
            curSliceCounter = 0;
        }
    }

    return greatestSliceStart;
}