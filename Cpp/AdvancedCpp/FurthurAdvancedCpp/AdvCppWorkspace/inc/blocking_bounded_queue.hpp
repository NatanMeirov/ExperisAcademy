#ifndef NM_BLOCKING_BOUNDED_QUEUE_HPP
#define NM_BLOCKING_BOUNDED_QUEUE_HPP


#include <cstddef> // size_t
#include <deque>
#include <mutex>
#include <semaphore.h>
#include "atomic_value.hpp"


namespace advcpp
{

// Concept of T: MUST be copy-constructable, copy-assignable and default-constructable
// Concept of DestructionPolicy: policy must be copy-constructable
// The destruction policy is a FUNCTOR (implements operator() and get 1 param: BlockingBoundedQueue& obj), to be used as an instructions to know which action the BlockingBoundedQueue
// object should call on itself when it is in a destruction stage
// Note: The BlockingBoundedQueue<T> class has exception safe operations
template <typename T, typename DestructionPolicy>
class BlockingBoundedQueue
{
    friend DestructionPolicy;
public:
    BlockingBoundedQueue(size_t a_initialCapacity, DestructionPolicy a_destructionPolicy); // DestructionPolicy is a lightweight object (can get it by value (copy))
    BlockingBoundedQueue(const BlockingBoundedQueue& a_other) = delete;
    BlockingBoundedQueue& operator=(const BlockingBoundedQueue& a_other) = delete;
    ~BlockingBoundedQueue();

    // Returns false if the queue is closed and no furthur operations can be done with it
    bool Enqueue(const T& a_item);
    bool Dequeue(T& a_itemToReturnByRef);

    size_t Size() const; // Returns 0 if queue is not valid
    size_t Capacity() const; // Returns 0 if queue is not valid
    bool IsEmpty() const; // Returns true if queue is not valid
    bool IsFull() const; // Returns false if queue is not valid

private:
    void InitializeSemaphores(size_t a_initialCapacity);
    void ReleaseSemaphores();
    void DestroySemaphores();
    void Close();
    bool IsClosed() const;
    void LockOperations();
    bool ShouldNotOperate() const;

    // For policy uses (without locking)
    T RemoveNext() noexcept;
    bool Empty() const noexcept;
    size_t GetSize() const noexcept;

private:
    std::deque<T> m_queue;
    mutable std::mutex m_mutex;
    sem_t m_freeSlots;
    sem_t m_occupiedSlots;
    DestructionPolicy m_destructionPolicy;
    size_t m_capacity;
    AtomicValue<size_t> m_enqueueWaiters;
    AtomicValue<size_t> m_dequeueWaiters;
    AtomicFlag m_isValid;
    AtomicFlag m_areOperationsLocked;
};

} // advcpp


#include "inl/blocking_bounded_queue.hxx"


#endif // NM_BLOCKING_BOUNDED_QUEUE_HPP
