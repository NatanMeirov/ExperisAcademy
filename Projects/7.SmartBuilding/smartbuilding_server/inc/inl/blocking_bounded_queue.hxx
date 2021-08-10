#ifndef NM_BLOCKING_BOUNDED_QUEUE_HXX
#define NM_BLOCKING_BOUNDED_QUEUE_HXX


#include <cstddef> // size_t
#include <memory> // std::shared_ptr, std::make_shared
#include <deque>
#include <mutex>
#include <stdexcept> // std::runtime_error
#include "semaphore.hpp"
#include "barrier.hpp"
#include "atomic_value.hpp"


namespace advcpp
{

template <typename T, typename DestructionPolicy>
BlockingBoundedQueue<T,DestructionPolicy>::BlockingBoundedQueue(size_t a_initialCapacity, DestructionPolicy a_destructionPolicy)
: m_queue()
, m_mutex()
, m_freeSlots(a_initialCapacity)
, m_occupiedSlots(0)
, m_enqueueWaitersBarrier(1) // Dummy value - should be reset at the destruction
, m_dequeueWaitersBarrier(1) // Dummy value - should be reset at the destruction
, m_destructionPolicy(a_destructionPolicy)
, m_size(0)
, m_capacity(a_initialCapacity)
, m_enqueueWaiters(0)
, m_dequeueWaiters(0)
, m_isValid(true)
{
    if(!a_initialCapacity)
    {
        throw std::runtime_error("Initial capacity cannot be zero");
    }
}


template <typename T, typename DestructionPolicy>
BlockingBoundedQueue<T,DestructionPolicy>::~BlockingBoundedQueue()
{
    Close();
    ReleaseAllBlockedWaiters();
    // Lock the mutex
    std::lock_guard<std::mutex> lock(m_mutex);
    m_destructionPolicy(*this);
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::Enqueue(const T& a_item)
{
    if(IsClosed())
    {
        return false;
    }

    ++m_enqueueWaiters;
    m_freeSlots.Down(); // -1
    --m_enqueueWaiters;

    if(IsClosed()) // Double check lock
    {
        m_enqueueWaitersBarrier.Wait();
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        try
        {
            m_queue.push_back(a_item); // Exception prone code - copy-constructor may fail
            ++m_size;
        }
        catch(...) // Exception safety: keeping the correct class' invariants
        {
            m_freeSlots.Up(); // +1

            throw; // rethrow
        }
    }
    m_occupiedSlots.Up(); // +1

    return true;
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::Dequeue(T& a_itemToReturnByRef)
{
    if(IsClosed())
    {
        return false;
    }

    ++m_dequeueWaiters;
    m_occupiedSlots.Down(); // -1
    --m_dequeueWaiters;

    if(IsClosed()) // Double check lock
    {
        m_dequeueWaitersBarrier.Wait();
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        try
        {
            a_itemToReturnByRef = m_queue.front(); // Exception prone code - copy-assignment may fail
            m_queue.pop_front();
            --m_size;
        }
        catch(...) // Exception safety: keeping the correct class' invariants
        {
            m_occupiedSlots.Up(); // +1

            throw; // rethrow
        }
    }
    m_freeSlots.Up(); // +1

    return true;
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Size() const
{
    if(IsClosed())
    {
        return 0;
    }

    return m_size.Get();
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Capacity() const
{
    if(IsClosed())
    {
        return 0;
    }

    return m_capacity; // This value isn't changing - no race condition here (only read operations)
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsEmpty() const
{
    if(IsClosed())
    {
        return true;
    }

    return m_size.Get() == 0;
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsFull() const
{
    if(IsClosed())
    {
        return false;
    }

    return m_size.Get() == m_capacity;
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::ReleaseAllBlockedWaiters()
{
    m_enqueueWaitersBarrier.Reset(m_enqueueWaiters.Get() + 1); // +1 stands for the queue itself
    m_dequeueWaitersBarrier.Reset(m_dequeueWaiters.Get() + 1); // +1 stands for the queue itself

    // Release Enqueue waiters:
    while(m_enqueueWaiters.Get() > 0)
    {
        m_freeSlots.Up(); // +1
        // The Enqueue method handles the --m_enqueueWaiters;
    }

    // Release Dequeue waiters:
    while(m_dequeueWaiters.Get() > 0)
    {
        m_occupiedSlots.Up(); // +1
        // The Dequeue method handles the --m_dequeueWaiters;
    }

    // Waiting for the threads to exit completely (to avoid race condition at the destruction of the queue):
    m_enqueueWaitersBarrier.Wait();
    m_dequeueWaitersBarrier.Wait();
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::Close()
{
    m_isValid.False();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsClosed() const
{
    return !m_isValid.Check();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::RemoveNext(T& a_itemToReturnByRef) noexcept
{
    try // Exception safety
    {
        a_itemToReturnByRef = m_queue.front();
        m_queue.pop_front();
        --m_size;
    }
    catch(...)
    {
        return false;
    }

    return true;
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::Empty() const noexcept
{
    return m_size.Get() == 0;
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::GetSize() const noexcept
{
    return m_size.Get();
}

}


#endif // NM_BLOCKING_BOUNDED_QUEUE_HXX
