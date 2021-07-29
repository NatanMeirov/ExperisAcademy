#ifndef NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP
#define NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP


#include <memory> // std::shared_ptr
#include "blocking_bounded_queue.hpp"


namespace advcpp
{
// Policies to be triggered when a BlockingBoundedQueue is destructed.
// All the Policies MUST NOT throw exceptions (must be nothrow (noexcept))!
// Policies that wants to remove single item or check conditions, should only use private method (Policies declaired as friends of BlockingBoundedQueue)


// AssertPolicy: Asserts that the queue is empty on a destruction
// Concept of T: T must be the same type like BlockingBoundedQueue's T
template<typename T>
class AssertPolicy
{
public:
    void operator()(BlockingBoundedQueue<T,AssertPolicy<T>>& a_queue) noexcept;
};


// NoOperationPolicy: Do nothing (NoOP) on the queue's destruction
// Concept of T: T must be the same type like BlockingBoundedQueue's T
template<typename T>
class NoOperationPolicy
{
public:
    void operator()(BlockingBoundedQueue<T,NoOperationPolicy<T>>& a_queue) noexcept;
};


// ClearPolicy: Clears the queue from its remained items on a destruction
// Concept of T: T must be the same type like BlockingBoundedQueue's T
template<typename T>
class ClearPolicy
{
public:
    void operator()(BlockingBoundedQueue<T,ClearPolicy<T>>& a_queue) noexcept;
};


// SavePolicy: Saves the remained item in a given shared_ptr to a container (C),
// using push_back method to insert them to the END of the container, on the queue's destruction
// Concept of T: T must be the same type like BlockingBoundedQueue's T
// Concept of C: C must be a container that its value type is T, that implements push_back method
template<typename T, typename C>
class SavePolicy
{
public:
    SavePolicy(std::shared_ptr<C> a_containerPtr) : m_containerPtr(a_containerPtr) {}

    void operator()(BlockingBoundedQueue<T,SavePolicy<T,C>>& a_queue) noexcept;

private:
    std::shared_ptr<C> m_containerPtr;
};


// CallbackPolicy: Calls to Func on each remained item of the queue on its destruction (uses Dequeue on each of them)
// Concept of T: T must be the same type like BlockingBoundedQueue's T
// Concept of Func: Func must copy-constructable, and must be callable that gets T as a parameter
template<typename T, typename Func>
class CallbackPolicy
{
public:
    CallbackPolicy(Func a_func) : m_func(a_func) {}

    void operator()(BlockingBoundedQueue<T,CallbackPolicy<T,Func>>& a_queue) noexcept;

private:
    Func m_func;
};

} // advcpp


#include "inl/blocking_bounded_queue_destruction_policies.hxx"


#endif // NM_BLOCKING_BOUNDED_QUEUE_DESTRUCTION_POLICIES_HPP