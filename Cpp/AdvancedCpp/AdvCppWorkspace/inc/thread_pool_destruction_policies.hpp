#ifndef NM_THREAD_POOL_DESTRUCTION_POLICIES_HPP
#define NM_THREAD_POOL_DESTRUCTION_POLICIES_HPP


#include <memory> // std::shared_ptr
#include "thread_pool.hpp"
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace advcpp
{
// Policies to be triggered when a BlockingBoundedQueue is destructed.
// All the Policies MUST NOT throw exceptions (must be nothrow (noexcept))!
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueTypeDestructionPolicy: must be a destruction policy of the given Queue type, and must be a destruction policy of type T = std::shared_ptr<ICallable>
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueType: QueueType must implement Enqueue and Dequeue methods (Suggestion: these methods should be multithreaded-safe!),
// and its T MUST be std::shared_ptr of type ICallable (QueueType< T = std::shared_ptr<ICallable> >),
// and it must implement a C'tor of: {size_t, QueueTypeDestructionPolicy<std::shared_ptr<ICallable>>}


template <typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class AssertingPolicy
{
public:
    void operator()(ThreadPool<AssertingPolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept;
};


template <typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class ShutdownPolicy
{
public:
    void operator()(ThreadPool<ShutdownPolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept;
};


template <typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class ShutdownImmediatePolicy
{
public:
    void operator()(ThreadPool<ShutdownImmediatePolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept;
};

} // advcpp


#include "inl/thread_pool_destruction_policies.hxx"


#endif // NM_THREAD_POOL_DESTRUCTION_POLICIES_HPP