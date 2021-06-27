#ifndef __NM_SAFEQUEUE_INLINE_HPP__
#define __NM_SAFEQUEUE_INLINE_HPP__


#include "SafeQueue.hpp"
#include <cstddef> // size_t
#include <queue>
#include "Mutex.hpp"
#include "LockGuard.hpp"


template <typename T>
nm::SafeQueue<T>::SafeQueue(const size_t a_initialCapacity)
: m_queue()
, m_lock()
, m_currentFreePlacesInQueue(SafeQueue<T>::SEMAPHORE_THREAD_OPTION, a_initialCapacity)
, m_currentOccupiedPlacesInQueue(SafeQueue<T>::SEMAPHORE_THREAD_OPTION, 0) {
}


template <typename T>
void nm::SafeQueue<T>::Enqueue(const T& a_item) {
    this->m_currentFreePlacesInQueue.Down();
    nm::LockGuard(this->m_lock);
    this->m_queue.push(a_item);
    this->m_currentOccupiedPlacesInQueue.Up();
}


template <typename T>
T nm::SafeQueue<T>::Dequeue() {
    this->m_currentOccupiedPlacesInQueue.Down();
    nm::LockGuard(this->m_lock);
    T popedItem = this->m_queue.front();
    this->m_queue.pop();
    this->m_currentFreePlacesInQueue.Up();

    return popedItem;
}


template <typename T>
bool nm::SafeQueue<T>::IsEmpty() {
    nm::LockGuard(this->m_lock);
    return this->m_queue.empty();
}


template <typename T>
size_t nm::SafeQueue<T>::Size() {
    nm::LockGuard(this->m_lock);
    return this->m_queue.size();
}


template <typename T>
T& nm::SafeQueue<T>::GetFront() {
    nm::LockGuard(this->m_lock);
    return this->m_queue.front();
}


template <typename T>
T& nm::SafeQueue<T>::GetBack() {
    nm::LockGuard(this->m_lock);
    return this->m_queue.back();
}


#endif // __NM_SAFEQUEUE_INLINE_HPP__