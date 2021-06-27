#ifndef __NM_QUEUE_INLINE_HPP__
#define __NM_QUEUE_INLINE_HPP__


#include "Queue.hpp"
#include <stdexcept>



namespace nm {

template <typename T>
Queue<T>::Queue(const size_t a_initialCapacity)
: m_queue(new T[a_initialCapacity])
, m_headIndex(0)
, m_tailIndex(0)
, m_currentSize(0)
, m_maxCapacity(a_initialCapacity) {
}


template <typename T>
Queue<T>::Queue(const Queue& a_other)
: m_queue(new T[a_other.m_maxCapacity])
, m_headIndex(a_other.m_headIndex)
, m_tailIndex(a_other.m_tailIndex)
, m_currentSize(a_other.m_currentSize)
, m_maxCapacity(a_other.m_maxCapacity){
    for(size_t location = a_other.m_headIndex, totalIterations = 0; totalIterations < a_other.m_currentSize; ++totalIterations, location = (location + 1) % a_other.m_maxCapacity) {
        this->m_queue[location] = a_other.m_queue[location];
    }
}


template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& a_other) {
    if(this != &a_other) { // Saving from self assignment
        T* newQueue = new T[a_other.m_maxCapacity]; // Exception safety

        delete[] this->m_queue;

        this->m_queue = newQueue;
        this->m_headIndex = a_other.m_headIndex;
        this->m_tailIndex = a_other.m_tailIndex;
        this->m_currentSize = a_other.m_currentSize;
        this->m_maxCapacity = a_other.m_maxCapacity;

        for(size_t location = a_other.m_headIndex, totalIterations = 0; totalIterations < a_other.m_currentSize; ++totalIterations, location = (location + 1) % a_other.m_maxCapacity) {
            this->m_queue[location] = a_other.m_queue[location];
        }
    }

    return *this;
}


template <typename T>
Queue<T>::~Queue() {
    delete[] this->m_queue;
}


template <typename T>
void Queue<T>::Enqueue(const T& a_itemToPush) {
    if(this->m_currentSize == this->m_maxCapacity) {
        throw std::overflow_error("Queue reached its maximum capacity, cannot add another item");
    }

    this->m_queue[this->m_tailIndex] = a_itemToPush;
    this->m_tailIndex = (this->m_tailIndex + 1) % this->m_maxCapacity;
    ++this->m_currentSize;
}


template <typename T>
T Queue<T>::Dequeue() {
    if(this->m_currentSize == 0) {
        throw std::underflow_error("Queue is empty, there are no items to pop out");
    }

    T itemToPop = this->m_queue[this->m_headIndex];
    this->m_headIndex = (this->m_headIndex + 1) % this->m_maxCapacity;
    --this->m_currentSize;

    return itemToPop;
}


template <typename T>
T& Queue<T>::Front() {
    if(this->m_currentSize == 0) {
        throw std::runtime_error("Queue is empty");
    }

    return this->m_queue[this->m_headIndex];
}


template <typename T>
T& Queue<T>::Back() {
    if(this->m_currentSize == 0) {
        throw std::runtime_error("Queue is empty");
    }

    return this->m_queue[this->m_tailIndex];
}


template <typename T>
size_t Queue<T>::Size() {
    return this->m_currentSize;
}


template <typename T>
bool Queue<T>::IsEmpty() {
    return this->m_currentSize == 0;
}

} // nm


#endif // __NM_QUEUE_INLINE_HPP__