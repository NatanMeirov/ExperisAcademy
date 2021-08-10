#ifndef NM_THREAD_POOL_DESTRUCTION_POLICIES_HXX
#define NM_THREAD_POOL_DESTRUCTION_POLICIES_HXX


#include <cassert> // assert


namespace advcpp
{

template <typename QueueTypeDestructionPolicy, typename QueueType>
void AssertingPolicy<QueueTypeDestructionPolicy,QueueType>::operator()(ThreadPool<AssertingPolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept
{
    assert(a_pool.HasStopped());
}


template <typename QueueTypeDestructionPolicy, typename QueueType>
void ShutdownPolicy<QueueTypeDestructionPolicy,QueueType>::operator()(ThreadPool<ShutdownPolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept
{
    try
    {
        a_pool.ConditionalShutdown();
    }
    catch (...)
    {
    }
}


template <typename QueueTypeDestructionPolicy, typename QueueType>
void ShutdownImmediatePolicy<QueueTypeDestructionPolicy,QueueType>::operator()(ThreadPool<ShutdownImmediatePolicy, QueueTypeDestructionPolicy, QueueType>& a_pool) noexcept
{
    try
    {
        a_pool.ConditionalShutdownImmidiate();
    }
    catch (...)
    {
    }
}

} // advcpp


#endif // NM_THREAD_POOL_DESTRUCTION_POLICIES_HXX