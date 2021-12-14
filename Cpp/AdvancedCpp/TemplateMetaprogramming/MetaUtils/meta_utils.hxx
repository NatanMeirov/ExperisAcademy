#ifndef NM_META_UTILS_HXX
#define NM_META_UTILS_HXX


#include <cstddef> // size_t


namespace nm
{

namespace meta
{
// ------------------------------------------------------------------------------------------------
// For loop (forward):
// [FROM, TO),
// FUNC should be a callable functor (operator() implemented) or function pointer
template <size_t FROM, size_t TO>
class For
{
private:
    enum { go = (FROM + 1 < TO) };

public:
    template <typename FUNC>
    static inline void loop(FUNC func)
    {
        func();
        For<(go ? (FROM + 1) : 0), (go ? TO : 0)>::loop(func);
    }
};


// A specialization to end up the For<> recursion
template <>
class For<0,0>
{
public:
    template <typename FUNC>
    static inline void loop(FUNC func)
    {
        (void)(func);
    }
};


// ------------------------------------------------------------------------------------------------
// While loop:
// FUNC should be a callable functor (operator() implemented) or function pointer
template <size_t ITERATIONS>
class While
{
public:
    template <typename FUNC>
    static inline void loop(FUNC func)
    {
        func();
        While<(ITERATIONS - 1)>::loop(func);
    }
};


// A specialization to end up the While<> recursion
template <>
class While<0>
{
public:

    template <typename FUNC>
    static inline void loop(FUNC func)
    {
        (void)(func);
    }
};


// ------------------------------------------------------------------------------------------------
// If/Else conditional statement:
// FUNC_THEN/FUNC_ELSE should be a callable functor (operator() implemented) or function pointer
template <bool CONDITION>
class If
{
public:
    template <typename FUNC_THEN, typename FUNC_ELSE>
    static inline void evaluate(FUNC_THEN trueDecisionFunc, FUNC_ELSE falseDecisionFunc)
    {
        (void)(falseDecisionFunc);
        trueDecisionFunc();
    }
};


template <>
class If<false>
{
public:
    template <typename FUNC_THEN, typename FUNC_ELSE>
    static inline void evaluate(FUNC_THEN trueDecisionFunc, FUNC_ELSE falseDecisionFunc)
    {
        (void)(trueDecisionFunc);
        falseDecisionFunc();
    }
};


// ------------------------------------------------------------------------------------------------
// Bubble Sort:

template <typename T>
inline void swap(T& first, T& second)
{
    T holder(first);
    first = second;
    second = holder;
}


// A swapper class that gets 2 indexes and compare T (data) items, and perform a swapping if needed
template <typename T, size_t I, size_t J>
class Swapper
{
public:
    static inline void compareAndSwap(T* data)
    {
        if(data[I] > data[J])
        {
            swap(data[I], data[J]);
        }
    }
};


// A bubble sort loop class - to represent recursive loops on T* data array
template <typename T, size_t CURRENT, size_t LAST>
class BubbleSortLoop
{
private:
    enum { go = (CURRENT < (LAST - 1)) }; // Stops when 1 place before the end (comparing between {one before the last} and {last} items)

public:
    static inline void loop(T* data)
    {
        Swapper<T, CURRENT, (CURRENT + 1)>::compareAndSwap(data);
        BubbleSortLoop<T, (go ? (CURRENT + 1) : 0), (go ? LAST : 0)>::loop(data);
    }
};


// A specialization to end up the BubbleSortLoop<T> recursion
template <typename T>
class BubbleSortLoop<T,0,0>
{
public:
    static inline void loop(T* data)
    {
        (void)(data);
    }
};


// Concept of T: T should be a the type of the pointer of the array to sort,
// T must be copy-constructable, copy-assignable, and must implement operator> [is greater operator] (for comparisons)
template <typename T, size_t SIZE>
class BubbleSort
{
public:
    static inline void sort(T* data)
    {
        BubbleSortLoop<T, 0, SIZE - 1>::loop(data); // Current loop (from 0 to (N - 1) while N is the current interval to sort)
        BubbleSort<T, (SIZE - 1)>::sort(data); // A recursive call to bubble sort
    }
};


// A specialization to end up the BubbleSort<T> recursion (1 item to sort -> data is sorted already)
template <typename T>
class BubbleSort<T,1>
{
public:
    static inline void sort(T* data)
    {
        (void)(data);
    }
};


} // meta

} // nm


#endif // NM_META_UTILS_HXX