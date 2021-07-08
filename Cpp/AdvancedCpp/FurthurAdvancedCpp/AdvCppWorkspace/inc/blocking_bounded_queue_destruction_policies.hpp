#ifndef NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP
#define NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP


#include "blocking_bounded_queue.hpp"


namespace advcpp
{

// Concept of T: T must be the same type like BlockingBoundedQueue's T
template<typename T>
class AssertPolicy
{
public:
    void operator()(BlockingBoundedQueue<T,AssertPolicy<T>>& a_queue);
};


// Concept of T: T must be the same type like BlockingBoundedQueue's T
template<typename T>
class ClearPolicy
{
public:
    void operator()(BlockingBoundedQueue<T,ClearPolicy<T>>& a_queue);
};

} // advcpp


#include "inl/blocking_bounded_queue_destruction_policies.hxx"


#endif // NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP