#ifndef __NM_SAFEQUEUE_INLINE_HPP__
#define __NM_SAFEQUEUE_INLINE_HPP__


#include "SafeQueue.hpp"
#include <cstddef> // size_t
#include "Queue_Inline.hpp"
#include "Mutex.hpp"
#include "LockGuard.hpp"


template <typename T>
nm::SafeQueue<T>::SafeQueue(const size_t a_initialCapacity)
: m_queue(a_initialCapacity)
, m_lock()
, m_currentFreePlacesInQueue(SafeQueue<T>::SEMAPHORE_THREAD_OPTION, a_initialCapacity)
, m_currentOccupiedPlacesInQueue(SafeQueue<T>::SEMAPHORE_THREAD_OPTION, 0) {
}


template <typename T>
void nm::SafeQueue<T>::Enqueue(const T& a_item) {
    this->m_currentFreePlacesInQueue.Down();
    this->m_lock.Lock();
    this->m_queue.Enqueue(a_item);
    this->m_lock.Unlock();
    this->m_currentOccupiedPlacesInQueue.Up();
}


template <typename T>
T nm::SafeQueue<T>::Dequeue() {
    this->m_currentOccupiedPlacesInQueue.Down();
    this->m_lock.Lock();
    T popedItem = this->m_queue.Dequeue();
    this->m_lock.Unlock();
    this->m_currentFreePlacesInQueue.Up();

    return popedItem;
}


template <typename T>
bool nm::SafeQueue<T>::IsEmpty() {
    nm::LockGuard guard(this->m_lock);
    return this->m_queue.IsEmpty();
}


template <typename T>
size_t nm::SafeQueue<T>::Size() {
    nm::LockGuard guard(this->m_lock);
    return this->m_queue.Size();
}


template <typename T>
T& nm::SafeQueue<T>::GetFront() {
    nm::LockGuard guard(this->m_lock);
    return this->m_queue.Front();
}


template <typename T>
T& nm::SafeQueue<T>::GetBack() {
    nm::LockGuard guard(this->m_lock);
    return this->m_queue.Back();
}


#endif // __NM_SAFEQUEUE_INLINE_HPP__
