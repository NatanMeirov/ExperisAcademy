#ifndef NM_BLOCKING_BOUNDED_QUEUE_HPP
#define NM_BLOCKING_BOUNDED_QUEUE_HPP


#include <cstddef> // size_t
#include <deque>
#include <mutex>
#include "semaphore.h"
#include "atomic_value.hpp"


namespace advcpp
{

// Concept of T: MUST be copy-constructable and default-constructable
// Concept of DestructionPolicy: policy must be copy-constructable
// The destruction policy is a FUNCTOR (implements operator() and get 1 param: BlockingBoundedQueue& obj), to be used as an instructions to know which action the BlockingBoundedQueue
// object should call on itself when it is in a destruction stage
// Note: The BlockingBoundedQueue<T> class has exception safe operations
template <typename T, typename DestructionPolicy>
class BlockingBoundedQueue
{
public:
    explicit BlockingBoundedQueue(size_t a_initialCapacity, const DestructionPolicy& a_destructionPolicy);
    BlockingBoundedQueue(const BlockingBoundedQueue& a_other) = delete;
    BlockingBoundedQueue& operator=(const BlockingBoundedQueue& a_other) = delete;
    ~BlockingBoundedQueue();

    void Enqueue(const T& a_item);
    T Dequeue();

    size_t Size() const;
    size_t Capacity() const;
    bool IsEmpty() const;
    bool IsFull() const;

private:
    void InitializeSemaphores(size_t a_initialCapacity);
    void DestroySemaphores();

private:
    std::deque<T> m_queue;
    mutable std::mutex m_mutex;
    sem_t m_freeSlots;
    sem_t m_occupiedSlots;
    DestructionPolicy m_destructionPolicy;
    size_t m_capacity;
};

} // advcpp


#include "inl/blocking_bounded_queue.hxx"


#endif // NM_BLOCKING_BOUNDED_QUEUE_HPP
