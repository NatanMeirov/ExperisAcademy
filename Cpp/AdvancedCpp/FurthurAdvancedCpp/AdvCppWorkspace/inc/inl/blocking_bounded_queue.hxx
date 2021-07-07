#ifndef NM_BLOCKING_BOUNDED_QUEUE_HXX
#define NM_BLOCKING_BOUNDED_QUEUE_HXX


#include <cstddef> // size_t
#include <memory> // std::shared_ptr, std::make_shared
#include "queue.hpp"
#include "mutex.hpp"
#include "lock_guard.hpp"


namespace advcpp
{

template <typename T>
BlockingBoundedQueue<T>::BlockingBoundedQueue(size_t a_initialCapacity)
: m_queue(a_initialCapacity)
, m_lock()
, m_freeSlots(a_initialCapacity)
, m_occupiedSlots(0)
{
}


template <typename T>
void BlockingBoundedQueue<T>::Enqueue(const T& a_item)
{
    m_freeSlots.Down();
    m_lock.Lock();

    try // Exception safety
    {
        m_queue.Enqueue(a_item);
    }
    catch(...) // Keeping the class' invariants
    {
        m_freeSlots.Up();
        m_lock.Unlock();
        throw; // rethrow
    }

    m_lock.Unlock();
    m_occupiedSlots.Up();
}


template <typename T>
T BlockingBoundedQueue<T>::Dequeue()
{
    m_occupiedSlots.Down();
    m_lock.Lock();

    size_t itemsBeforeDequeue = m_queue.Size();
    std::shared_ptr<T> poppedItem; // Instead of using T poppedItem; (Enforcing the user to use only default-constructable types as T)
    try // Exception safety
    {
        poppedItem = std::make_shared<T>(m_queue.Dequeue());
    }
    catch(...) // Saving the class' invariants
    {
        if(itemsBeforeDequeue == m_queue.Size()) // Item has NOT removed at all - must keep the correct class' invariants
        {
            m_occupiedSlots.Up();
        }
        else // Item has removed successfully
        {
            m_freeSlots.Up();
        }

        m_lock.Unlock();
        throw; // rethrow
    }

    m_lock.Unlock();
    m_freeSlots.Up();

    return *poppedItem;
}


template <typename T>
size_t BlockingBoundedQueue<T>::Size() const
{
    LockGuard guard(m_lock);
    return m_queue.Size();
}


template <typename T>
size_t BlockingBoundedQueue<T>::Capacity() const
{
    LockGuard guard(m_lock);
    return m_queue.Capacity();
}


template <typename T>
bool BlockingBoundedQueue<T>::IsEmpty() const
{
    LockGuard guard(m_lock);
    return m_queue.IsEmpty();
}


template <typename T>
bool BlockingBoundedQueue<T>::IsFull() const
{
    LockGuard guard(m_lock);
    return m_queue.IsFull();
}

}


#endif // NM_BLOCKING_BOUNDED_QUEUE_HXX