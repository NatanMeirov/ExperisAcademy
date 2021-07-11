#ifndef NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX
#define NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX



#include <cstddef> // size_t
#include <cassert> // assert
#include "blocking_bounded_queue.hpp"


namespace advcpp
{

template<typename T>
void AssertPolicy<T>::operator()(BlockingBoundedQueue<T,AssertPolicy<T>>& a_queue) noexcept
{
    assert(a_queue.Empty());
}


template<typename T>
void NoOperationPolicy<T>::operator()(BlockingBoundedQueue<T,NoOperationPolicy<T>>& a_queue) noexcept
{
    (void)a_queue; // Do nothing (not using a_queue at all)
}


template<typename T>
void ClearPolicy<T>::operator()(BlockingBoundedQueue<T,ClearPolicy<T>>& a_queue) noexcept
{
    size_t itemsToPop = a_queue.GetSize();
    for(size_t i = 0; i < itemsToPop; ++i)
    {
        try
        {
            a_queue.RemoveNext();
        }
        catch(...)
        { // Exception safe
        }
    }
}


template<typename T, typename C>
void SavePolicy<T,C>::operator()(BlockingBoundedQueue<T,SavePolicy<T,C>>& a_queue) noexcept
{
    size_t itemsToPop = a_queue.GetSize();
    for(size_t i = 0; i < itemsToPop; ++i)
    {
        try
        {
            m_containerPtr->push_back(a_queue.RemoveNext());
        }
        catch(...)
        { // Exception safe
        }
    }
}


template<typename T, typename Func>
void CallbackPolicy<T,Func>::operator()(BlockingBoundedQueue<T,CallbackPolicy<T,Func>>& a_queue) noexcept
{
    size_t itemsToPop = a_queue.GetSize();
    for(size_t i = 0; i < itemsToPop; ++i)
    {
        try
        {
            m_func(a_queue.RemoveNext());
        }
        catch(...)
        { // Exception safe
        }
    }
}

} // advcpp


#endif // NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX