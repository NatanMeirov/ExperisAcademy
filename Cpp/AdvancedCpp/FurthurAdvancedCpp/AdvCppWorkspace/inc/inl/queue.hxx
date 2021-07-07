#ifndef NM_QUEUE_HXX
#define NM_QUEUE_HXX


#include <stdexcept> // std::overflow_error


namespace advcpp
{

template <typename T>
Queue<T>::Queue(size_t a_initialCapacity)
: m_queue(new T[a_initialCapacity])
, m_headIndex(0)
, m_tailIndex(0)
, m_currentSize(0)
, m_maxCapacity(a_initialCapacity)
{
}


template <typename T>
Queue<T>::Queue(const Queue& a_other)
: m_queue(new T[a_other.m_maxCapacity])
, m_headIndex(a_other.m_headIndex)
, m_tailIndex(a_other.m_tailIndex)
, m_currentSize(a_other.m_currentSize)
, m_maxCapacity(a_other.m_maxCapacity)
{
    Copy(a_other);
}


template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& a_other)
{
    if(this != &a_other) // Saving from self assignment
    {
        T* newQueue = new T[a_other.m_maxCapacity]; // Exception safety
        delete[] m_queue;

        m_queue = newQueue;
        m_headIndex = a_other.m_headIndex;
        m_tailIndex = a_other.m_tailIndex;
        m_currentSize = a_other.m_currentSize;
        m_maxCapacity = a_other.m_maxCapacity;

        Copy(a_other);
    }

    return *this;
}


template <typename T>
Queue<T>::~Queue()
{
    delete[] m_queue;
}


template <typename T>
void Queue<T>::Enqueue(const T& a_itemToPush)
{
    if(m_currentSize == m_maxCapacity)
    {
        throw std::overflow_error("Queue reached its maximum capacity, cannot add another item");
    }

    m_queue[m_tailIndex] = a_itemToPush;
    m_tailIndex = (m_tailIndex + 1) % m_maxCapacity;
    ++m_currentSize;
}


template <typename T>
T Queue<T>::Dequeue()
{
    if(m_currentSize == 0)
    {
        throw std::underflow_error("Queue is empty, there are no items to pop out");
    }

    T itemToPop = m_queue[m_headIndex];
    m_headIndex = (m_headIndex + 1) % m_maxCapacity;
    --m_currentSize;

    return itemToPop;
}


template <typename T>
T& Queue<T>::Front()
{
    if(m_currentSize == 0)
    {
        throw std::runtime_error("Queue is empty");
    }

    return m_queue[m_headIndex];
}


template <typename T>
T& Queue<T>::Back()
{
    if(m_currentSize == 0)
    {
        throw std::runtime_error("Queue is empty");
    }

    return m_queue[m_tailIndex];
}


template <typename T>
const T& Queue<T>::Front() const
{
    if(m_currentSize == 0)
    {
        throw std::runtime_error("Queue is empty");
    }

    return m_queue[m_headIndex];
}


template <typename T>
const T& Queue<T>::Back() const
{
    if(m_currentSize == 0)
    {
        throw std::runtime_error("Queue is empty");
    }

    return m_queue[m_tailIndex];
}


template <typename T>
size_t Queue<T>::Size() const
{
    return m_currentSize;
}


template <typename T>
size_t Queue<T>::Capacity() const
{
    return m_maxCapacity;
}


template <typename T>
bool Queue<T>::IsEmpty() const
{
    return m_currentSize == 0;
}


template <typename T>
bool Queue<T>::IsFull() const
{
    return m_currentSize == m_maxCapacity;
}


template <typename T>
void Queue<T>::Copy(const Queue& a_other)
{
    for(size_t location = a_other.m_headIndex, totalIterations = 0; totalIterations < a_other.m_currentSize; ++totalIterations, location = (location + 1) % a_other.m_maxCapacity)
    {
        m_queue[location] = a_other.m_queue[location];
    }
}

} // advcpp


#endif // NM_QUEUE_HXX