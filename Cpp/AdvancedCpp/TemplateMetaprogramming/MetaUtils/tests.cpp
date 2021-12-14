#include "meta_utils.hxx"
#include <cstddef>
#include <iostream>
using namespace nm::meta;

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

void printChar(char c)
{
    std::cout << c << std::endl;
}

void printInt(int n)
{
    std::cout << n << std::endl;
}

int main()
{
    constexpr size_t N = 10;

    int* arr = new int[N]{4,7,-1,9,0,-33,1,56,333,-100};
    BubbleSort<int, N>::sort(arr);

    unsigned int* arr2 = new unsigned int[N]{4,7,1,9,0,33,1,56,333,100};
    BubbleSort<unsigned int, N>::sort(arr2);

    printArray<int>(arr, N);
    printArray<unsigned int>(arr2, N);


    std::cout << "While test (print 10 times the char '*'):" << std::endl;
    While<N>::loop([](){ printChar('*'); });

    std::cout << "For test (print from N/2 to N+N times the number N*N):" << std::endl;
    For<N/2,N+N>::loop([](){ printInt(N*N); });

    std::cout << "If test (first prints N*N, and then prints '*')" << std::endl;

    If<(N>N)>::evaluate([](){ printChar('*'); }, [](){ printInt(N*N); });
    If<(N==N)>::evaluate([](){ printChar('*'); }, [](){ printInt(N*N); });

    return 0;
}