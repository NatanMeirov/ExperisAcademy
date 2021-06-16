#ifndef __NM_SAFEQUEUE_INLINE_HPP__
#define __NM_SAFEQUEUE_INLINE_HPP__


#include "SafeQueue.hpp"
#include <cstddef> // size_t
#include <queue>
#include "Mutex.hpp"
#include "LockGuard.hpp"


template <typename T>
nm::SafeQueue<T>::SafeQueue(size_t a_initialCapacity)
: m_queue(a_initialCapacity)
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
void nm::SafeQueue<T>::Dequeue() {
    this->m_currentOccupiedPlacesInQueue.Down();
    nm::LockGuard(this->m_lock);
    this->m_queue.pop();
    this->m_currentFreePlacesInQueue.Up();
}


template <typename T>
bool nm::SafeQueue<T>::IsEmpty() {
    nm::LockGuard(this->m_lock);
    return this->m_queue.empty();
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