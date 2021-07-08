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
BlockingBoundedQueue<T,DestructionPolicy>::BlockingBoundedQueue(size_t a_initialCapacity, const DestructionPolicy& a_destructionPolicy)
: m_queue()
, m_mutex()
, m_freeSlots()
, m_occupiedSlots()
, m_destructionPolicy(a_destructionPolicy)
, m_capacity(a_initialCapacity)
{
    InitializeSemaphores(a_initialCapacity);
}


template <typename T, typename DestructionPolicy>
BlockingBoundedQueue<T,DestructionPolicy>::~BlockingBoundedQueue()
{
    DestroySemaphores();
    m_destructionPolicy(*this);
}


template <typename T, typename DestructionPolicy>
void BlockingBoundedQueue<T,DestructionPolicy>::Enqueue(const T& a_item)
{
    sem_wait(&m_freeSlots); // Down (-1)

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        size_t itemsBeforeEnqueue = m_queue.size();

        try
        {
            m_queue.push_back(a_item);
        }
        catch(...) // Exception safety: keeping the correct class' invariants
        {
            if(itemsBeforeEnqueue == m_queue.size()) // Item has NOT added at all
            {
                sem_post(&m_freeSlots); // Up (+1)
            }
            else // Item has added successfully
            {
                sem_post(&m_occupiedSlots); // Up (+1)
            }

            throw; // rethrow
        }
    }

    sem_post(&m_occupiedSlots); // Up (+1)
}


template <typename T, typename DestructionPolicy>
T BlockingBoundedQueue<T,DestructionPolicy>::Dequeue()
{
    sem_wait(&m_occupiedSlots); // Down (-1)

    T poppedItem;

    {
        std::lock_guard<std::mutex> lock(m_mutex); // RAII
        size_t itemsBeforeDequeue = m_queue.size();

        try // Exception safety
        {
            poppedItem = m_queue.front();
            m_queue.pop_front();
        }
        catch(...) // Saving the class' invariants
        {
            if(itemsBeforeDequeue == m_queue.size()) // Item has NOT removed at all
            {
                sem_post(&m_occupiedSlots); // Up (+1)
            }
            else // Item has removed successfully
            {
                sem_post(&m_freeSlots); // Up (+1)
            }

            throw; // rethrow
        }
    }

    sem_post(&m_freeSlots); // Up (+1)

    return poppedItem;
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Size() const
{
    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_queue.size();
}


template <typename T, typename DestructionPolicy>
size_t BlockingBoundedQueue<T,DestructionPolicy>::Capacity() const
{
    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_capacity;
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsEmpty() const
{
    std::lock_guard<std::mutex> lock(m_mutex); // RAII
    return m_queue.empty();
}


template <typename T, typename DestructionPolicy>
bool BlockingBoundedQueue<T,DestructionPolicy>::IsFull() const
{
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
void BlockingBoundedQueue<T,DestructionPolicy>::DestroySemaphores()
{
    assert(sem_destroy(&m_freeSlots) == 0 && sem_destroy(&m_occupiedSlots) == 0);
}

}


#endif // NM_BLOCKING_BOUNDED_QUEUE_HXX
