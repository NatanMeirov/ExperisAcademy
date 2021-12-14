#include <cstddef> // size_t
#include <iostream> // std::cout, std::endl
#include <cassert> // assert
#include <algorithm> // std::copy, std::sort, std::is_sorted


namespace advcpp
{

#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array[0])))


void PrintIntArray(int* a_array, size_t a_size)
{
    std::cout << "[ ";
    for(size_t i = 0; i < a_size; ++i)
    {
        std::cout << a_array[i] << " ";
    }
    std::cout << "]" << std::endl;
}



// Concept of T: T must be copy constructable and copy-assignable
template <typename T>
void Swap(T& a_first, T& a_second)
{
    T holder(a_first);
    a_first = a_second;
    a_second = holder;
}


// Naive solution:
// Merge two given arrays (A, B), into A
// Assumes that the last {size of B} numbers in the end of A - are not included (must be able to merge both to A)
// Note: Size of A must be greater than size of B
// Time-complexity: O(n*log(n))
void MergeTwoSortedArraysIntoFirst(int* a_arrayA, size_t a_sizeOfA, const int* a_arrayB, size_t a_sizeOfB)
{
    assert(a_sizeOfA > a_sizeOfB);
    assert(std::is_sorted(a_arrayA, a_arrayA + a_sizeOfA));
    assert(std::is_sorted(a_arrayB, a_arrayB + a_sizeOfB));

    std::copy(a_arrayB, a_arrayB + a_sizeOfB, a_arrayA + (a_sizeOfA - a_sizeOfB));
    std::sort(a_arrayA, a_arrayA + a_sizeOfA);
}

} // advcpp


// Naive main:
// int main()
// {
//     int arr[] = {1,3,6,7,8,99,1000,4444,233333, 1000000, 10000000};
//     int arr2[] = {-1,2,7,10,11,12,11111111};
//     advcpp::MergeTwoSortedArraysIntoFirst(arr, ARRAY_SIZE(arr), arr2, ARRAY_SIZE(arr2));
//     advcpp::PrintIntArray(arr, ARRAY_SIZE(arr));

//     return 0;
// }


// Best solution - Merge - O(n)
void MergeTwoSortedArraysIntoFirst(int* a_arrayA, size_t a_sizeOfA, const int* a_arrayB, size_t a_sizeOfB)
{
    assert(a_sizeOfA > a_sizeOfB);
    assert(std::is_sorted(a_arrayA, a_arrayA + a_sizeOfA));
    assert(std::is_sorted(a_arrayB, a_arrayB + a_sizeOfB));

    size_t fillArrayIndex = a_sizeOfA;
    size_t indexOfA = a_sizeOfA - a_sizeOfB;
    size_t indexOfB = a_sizeOfB;

    while(indexOfA > 0 && indexOfB > 0)
    {
        if(a_arrayA[indexOfA - 1] > a_arrayB[indexOfB - 1])
        {
            a_arrayA[fillArrayIndex--] = a_arrayA[indexOfA - 1];
            --indexOfA;
        }
        else
        {
            a_arrayA[fillArrayIndex--] = a_arrayB[indexOfB - 1];
            --indexOfB;
        }
    }

    if(indexOfA == 0)
    {
        while(indexOfB > 0)
        {
            a_arrayA[fillArrayIndex--] = a_arrayB[indexOfB - 1];
            --indexOfB;
        }
    }
}


// Best solution main:
int main()
{
    int arr[] = {1,3,6,7,8,99,1000,4444,233333, 1000000, 10000000};
    int arr2[] = {12,11111111};
    advcpp::MergeTwoSortedArraysIntoFirst(arr, ARRAY_SIZE(arr), arr2, ARRAY_SIZE(arr2));
    advcpp::PrintIntArray(arr, ARRAY_SIZE(arr));

    return 0;
}