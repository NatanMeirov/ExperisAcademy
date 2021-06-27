#ifndef __NM_QUEUE_HPP__
#define __NM_QUEUE_HPP__


#include <cstddef> // size_t


namespace nm {

// Concept of T: T must be copy-constructable
template <typename T>
class Queue {
public:
    Queue(const size_t a_initialCapacity);
    Queue(const Queue& a_other);
    Queue& operator=(const Queue& a_other);
    ~Queue();

    void Enqueue(const T& a_itemToPush);
    T Dequeue();
    T& Front();
    T& Back();
    size_t Size();
    bool IsEmpty();

private:
    T* m_queue;
    size_t m_headIndex;
    size_t m_tailIndex;
    size_t m_currentSize;
    size_t m_maxCapacity;
};

} // nm


#endif // __NM_QUEUE_HPP__