#include "meta_utils.hxx"
#include <cstddef>
#include <iostream>


// T must implement friend operator<< of ostream
template <typename T>
void printArray(T* array, size_t size)
{
    std::cout << "[ ";
    for(size_t i = 0; i < size; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << "]" << std::endl;
}


int main()
{
    constexpr size_t N = 10;

    int* arr = new int[N]{4,7,-1,9,0,-33,1,56,333,-100};
    nm::meta::BubbleSort<int, N>::sort(arr);

    unsigned int* arr2 = new unsigned int[N]{4,7,1,9,0,33,1,56,333,100};
    nm::meta::BubbleSort<unsigned int, N>::sort(arr2);

    printArray<int>(arr, N);
    printArray<unsigned int>(arr2, N);

    return 0;
}