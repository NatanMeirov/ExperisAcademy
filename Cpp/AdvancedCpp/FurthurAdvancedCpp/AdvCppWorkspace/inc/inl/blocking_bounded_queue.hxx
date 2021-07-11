#ifndef NM_BLOCKING_BOUNDED_QUEUE_HXX
#define NM_BLOCKING_BOUNDED_QUEUE_HXX


#include <cstddef> // size_t
#include <memory> // std::shared_ptr, std::make_shared
#include <semaphore.h> // sem_t and semaphore functions
#include <cassert> // assert
#include <deque>
#include <mutex>
#include <stdexcept> // std::runtime_error

#include "../blocking_bounded_queue.hpp"
namespace advcpp
{

template <typename T, typename DestructionPolicy>
BlockingBoundedQueue<T,DestructionPolicy>::BlockingBoundedQueue(size_t a_initialCapacity, DestructionPolicy a_destructionPolicy)
: m_queue()
, m_mutex()
, m_freeSlots()
, m_occupiedSlots()
, m_destructionPolicy(a_destructionPolicy)
, m_capacity(a_initialCapacity)
, m_enqueueWaiters(0)
, m_dequeueWaiters(0)
, m_isValid(true)
, m_areOperationsLocked(false)
{
    if(a_initialCapacity < 1)
    {
        throw std::runtime_error("Initial capacity cannot be negative, nor zero");
    }

    InitializeSemaphores(a_initialCapacity);
}


template <typename T, typename DestructionPolicy>
BlockingBoundedQueue<T,DestructionPolicy>::~BlockingBoundedQueue()
{
    Close();
    ReleaseSemaphores();
    m_destructionPolicy(*this);
    DestroySemaphores();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::Enqueue(const T& a_item)
{
    if(IsClosed())
    {
        return false;
    }

    ++m_enqueueWaiters;
    sem_wait(&m_freeSlots); // Down (-1)
    --m_enqueueWaiters;
    if(ShouldNotOperate()) // Double check lock
    {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        try
        {
            m_queue.push_back(a_item); // Exception prone code - copy-constructor may fail
        }
        catch(...) // Exception safety: keeping the correct class' invariants
        {
            sem_post(&m_freeSlots); // Up (+1)

            throw; // rethrow
        }
    }
    sem_post(&m_occupiedSlots); // Up (+1)

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
    sem_wait(&m_occupiedSlots); // Down (-1)
    --m_dequeueWaiters;
    if(ShouldNotOperate()) // Double check lock
    {
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        try
        {
            a_itemToReturnByRef = m_queue.front(); // Exception prone code - copy-assignment may fail
            m_queue.pop_front();
        }
        catch(...) // Exception safety: keeping the correct class' invariants
        {
            sem_post(&m_occupiedSlots); // Up (+1)

            throw; // rethrow
        }
    }
    sem_post(&m_freeSlots); // Up (+1)

    return true;
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Size() const
{
    if(IsClosed())
    {
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_queue.size();
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Capacity() const
{
    if(IsClosed())
    {
        return 0;
    }

    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_capacity;
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsEmpty() const
{
    if(IsClosed())
    {
        return true;
    }

    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_queue.empty();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsFull() const
{
    if(IsClosed())
    {
        return false;
    }

    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_queue.size() == m_capacity;
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::InitializeSemaphores(size_t a_initialCapacity)
{
    int statusCodes = ((sem_init(&m_freeSlots, 0, a_initialCapacity)) || (sem_init(&m_occupiedSlots, 0, 0)));
    if(statusCodes != 0)
    {
        throw std::runtime_error("Failed while tried to initialize a semaphore");
    }
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::ReleaseSemaphores()
{
    LockOperations();

    // Release Enqueue waiters:
    while(m_enqueueWaiters.Get() > 0)
    {
        sem_post(&m_freeSlots); // Up (+1)
        // The Enqueue method handles the --m_enqueueWaiters;
    }

    // Release Dequeue waiters:
    while(m_dequeueWaiters.Get() > 0)
    {
        sem_post(&m_occupiedSlots); // Up (+1)
        // The Dequeue method handles the --m_dequeueWaiters;
    }
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::DestroySemaphores()
{
    assert(sem_destroy(&m_freeSlots) == 0 && sem_destroy(&m_occupiedSlots) == 0);
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
void BlockingBoundedQueue<T,DestructionPolicy>::LockOperations()
{
    m_areOperationsLocked.True();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::ShouldNotOperate() const
{
    return m_areOperationsLocked.Check();
}


template <typename T, typename DestructionPolicy>
T BlockingBoundedQueue<T,DestructionPolicy>::RemoveNext() noexcept
{
    try // Exception safety
    {
        T itemToRemove = m_queue.front();
        m_queue.pop_front();

        return itemToRemove;
    }
    catch(...)
    { // Exception safety
    }
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::Empty() const noexcept
{
    return m_queue.empty();
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::GetSize() const noexcept
{
    return m_queue.size();
}

}


#endif // NM_BLOCKING_BOUNDED_QUEUE_HXX
