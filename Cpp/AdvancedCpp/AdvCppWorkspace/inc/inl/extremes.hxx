#ifndef NM_EXTREMES_HXX
#define NM_EXTREMES_HXX


#include <tuple>
#include <type_traits> // std::is_same
#include <functional> // std::less


namespace nm
{

namespace advcpp
{

template <typename FwdIterator>
static FwdIterator DoubleStep(FwdIterator a_itr)
{
    ++a_itr;
    return ++a_itr;
}


// Naive solution: O(2*n) (2 comparisons for each 1 item)
// template <typename C, typename T>
// std::tuple<T*, T*> FindMinMax(const C& a_container)
// {
//     static_assert(std::is_same<typename C::value_type, T>::value, "C::value_type must be of type T");

//     typename C::iterator itr = a_container.begin();
//     typename C::iterator endItr = a_container.end();

//     if(itr == endItr) // An empty container
//     {
//         return std::make_tuple(nullptr, nullptr);
//     }

//     T* min = &(*itr);
//     T* max = &(*itr);
//     while(++itr != endItr)
//     {
//         if(std::less<T>()(*itr, *min))
//         {
//             // Update min:
//             min = &(*itr);
//         }
//         else if(std::less<T>()(*max, *itr))
//         {
//             // Update max:
//             max = &(*itr);
//         }
//     }

//     return std::make_tuple(min, max);
// }


// Best solution: O(3/2*n) (3 comparisons for each 2 items)
template <typename C, typename T>
std::tuple<T*, T*> FindMinMax(const C& a_container)
{
    static_assert(std::is_same<typename C::value_type, T>::value, "C::value_type must be of type T");

    typename C::iterator oneStepItr = a_container.begin();
    typename C::iterator doubleStepItr = a_container.begin();
    typename C::iterator endItr = a_container.end();

    if(oneStepItr == endItr) // An empty container
    {
        return std::make_tuple(nullptr, nullptr);
    }

    T* min = &(*oneStepItr);
    T* max = &(*oneStepItr);

    while(++oneStepItr != endItr && (doubleStepItr = DoubleStep(doubleStepItr)) != endItr)
    {
        if(std::less<T>()(*oneStepItr, *doubleStepItr))
        {
            // Check if should be new min:
            if(std::less<T>()(*oneStepItr, *min))
            {
                min = &(*oneStepItr);
            }

            // Check if should be new max:
            if(std::less<T>()(*max, *doubleStepItr))
            {
                max = &(*doubleStepItr);
            }
        }
        else // std::less<T>()(*doubleStepItr, *oneStepItr)
        {
            // Check if should be new min:
            if(std::less<T>()(*doubleStepItr, *min))
            {
                min = &(*doubleStepItr);
            }

            // Check if should be new max:
            if(std::less<T>()(*max, *oneStepItr))
            {
                max = &(*oneStepItr);
            }
        }
    }

    // A special case for oneStepItr != endItr but doubleStepItr == endItr (and the while loop had stopped before checking the last item in the container):
    if(oneStepItr != endItr && doubleStepItr == endItr)
    {
        if(std::less<T>()(*oneStepItr, *min))
        {
            min = &(*oneStepItr);
        }
        else if(std::less<T>()(*max, *oneStepItr))
        {
            max = &(*oneStepItr);
        }
    }

    return std::make_tuple(min, max);
}


// Best solution: O(3/2*n) (3 comparisons for each 2 items)
template <typename FwdIterator>
std::tuple<FwdIterator, FwdIterator> FindMinMax(FwdIterator a_begin, FwdIterator a_end)
{
    FwdIterator oneStepItr = a_begin;
    FwdIterator doubleStepItr = a_begin;
    FwdIterator endItr = a_end;

    if(a_begin == a_end) // An empty container
    {
        return std::make_tuple(endItr, endItr);
    }

    FwdIterator minItr = oneStepItr;
    FwdIterator maxItr = oneStepItr;

    while(++oneStepItr != endItr && (doubleStepItr = DoubleStep(doubleStepItr)) != endItr)
    {
        if(std::less<typename FwdIterator::value_type>()(*oneStepItr, *doubleStepItr))
        {
            // Check if should be new min:
            if(std::less<typename FwdIterator::value_type>()(*oneStepItr, *minItr))
            {
                minItr = oneStepItr;
            }

            // Check if should be new max:
            if(std::less<typename FwdIterator::value_type>()(*maxItr, *doubleStepItr))
            {
                maxItr = doubleStepItr;
            }
        }
        else // std::less<typename FwdIterator::value_type>()(*doubleStepItr, *oneStepItr)
        {
            // Check if should be new min:
            if(std::less<typename FwdIterator::value_type>()(*doubleStepItr, *minItr))
            {
                minItr = doubleStepItr;
            }

            // Check if should be new max:
            if(std::less<typename FwdIterator::value_type>()(*maxItr, *oneStepItr))
            {
                maxItr = oneStepItr;
            }
        }
    }

    // A special case for oneStepItr != endItr but doubleStepItr == endItr (and the while loop had stopped before checking the last item in the container):
    if(oneStepItr != endItr && doubleStepItr == endItr)
    {
        if(std::less<typename FwdIterator::value_type>()(*oneStepItr, *minItr))
        {
            minItr = oneStepItr;
        }
        else if(std::less<typename FwdIterator::value_type>()(*maxItr, *oneStepItr))
        {
            maxItr = oneStepItr;
        }
    }

    return std::make_tuple(minItr, maxItr);
}


} // advcpp

} // nm


#endif // NM_EXTREMES_HXX