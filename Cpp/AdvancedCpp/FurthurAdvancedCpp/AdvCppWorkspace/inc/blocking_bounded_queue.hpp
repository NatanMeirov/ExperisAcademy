#ifndef NM_BLOCKING_BOUNDED_QUEUE_HPP
#define NM_BLOCKING_BOUNDED_QUEUE_HPP


#include <cstddef> // size_t
#include "queue.hpp"
#include "mutex.hpp"
#include "semaphore.hpp"


namespace advcpp
{

// Concept of T: MUST be copy-constructable, and heap-allocateable
// Note: The BlockingBoundedQueue<T> class has exception safe operations
template <typename T>
class BlockingBoundedQueue
{
public:
    explicit BlockingBoundedQueue(size_t a_initialCapacity);
    BlockingBoundedQueue(const BlockingBoundedQueue& a_other) = delete;
    BlockingBoundedQueue& operator=(const BlockingBoundedQueue& a_other) = delete;
    ~BlockingBoundedQueue() = default;

    void Enqueue(const T& a_item);
    T Dequeue();

    size_t Size() const;
    size_t Capacity() const;
    bool IsEmpty() const;
    bool IsFull() const;

private:
    Queue<T> m_queue;
    mutable Mutex m_lock;
    Semaphore m_freeSlots;
    Semaphore m_occupiedSlots;
};

} // advcpp


#include "inl/blocking_bounded_queue.hxx"


#endif // NM_BLOCKING_BOUNDED_QUEUE_HPP
