#ifndef NM_QUEUE_HPP
#define NM_QUEUE_HPP


#include <cstddef> // size_t


namespace advcpp
{

// Concept of T: T must be copy-constructable
template <typename T>
class Queue
{
public:
    explicit Queue(size_t a_initialCapacity);
    Queue(const Queue& a_other);
    Queue& operator=(const Queue& a_other);
    ~Queue();

    void Enqueue(const T& a_itemToPush);
    T Dequeue();
    T& Front();
    T& Back();
    const T& Front() const;
    const T& Back() const;
    size_t Size() const;
    size_t Capacity() const;
    bool IsEmpty() const;
    bool IsFull() const;

private:
    void Copy(const Queue& a_other);

private:
    T* m_queue;
    size_t m_headIndex;
    size_t m_tailIndex;
    size_t m_currentSize;
    size_t m_maxCapacity;
};

} // advcpp


#include "inl/queue.hxx"


#endif // NM_QUEUE_HPP