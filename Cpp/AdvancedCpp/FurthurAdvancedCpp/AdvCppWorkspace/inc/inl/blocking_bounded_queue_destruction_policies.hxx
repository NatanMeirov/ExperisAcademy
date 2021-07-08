#ifndef NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX
#define NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX



#include <cstddef> // size_t
#include <cassert> // assert
#include "blocking_bounded_queue.hpp"


namespace advcpp
{

template<typename T>
void AssertPolicy<T>::operator()(BlockingBoundedQueue<T,AssertPolicy<T>>& a_queue)
{
    assert(a_queue.IsEmpty());
}


template<typename T>
void ClearPolicy<T>::operator()(BlockingBoundedQueue<T,ClearPolicy<T>>& a_queue)
{
    size_t itemsToPop = a_queue.Size();
    for(size_t i = 0; i < itemsToPop; ++i)
    {
        a_queue.Dequeue();
    }
}

} // advcpp


#endif // NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HXX