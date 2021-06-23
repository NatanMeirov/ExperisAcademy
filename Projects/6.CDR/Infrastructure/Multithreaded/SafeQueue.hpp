#ifndef __NM_SAFEQUEUE_HPP__
#define __NM_SAFEQUEUE_HPP__


#include <cstddef> // size_t
#include <queue> // std::queue
#include "Mutex.hpp"
#include "Semaphore.hpp"


namespace nm {

// Concept of T: MUST be copy-constructable
template <typename T>
class SafeQueue {
public:
    explicit SafeQueue(const size_t a_initialCapacity);
    SafeQueue(const SafeQueue& a_other) = delete;
    SafeQueue& operator=(const SafeQueue& a_other) = delete;
    ~SafeQueue() = default;

    void Enqueue(const T& a_item);
    T Dequeue();
    bool IsEmpty();
    size_t Size();
    T& GetFront();
    T& GetBack();

private:
    static const int SEMAPHORE_THREAD_OPTION = 0;

    std::queue<T> m_queue;
    nm::Mutex m_lock;
    nm::Semaphore m_currentFreePlacesInQueue;
    nm::Semaphore m_currentOccupiedPlacesInQueue;
};

} // nm


#endif // __NM_SAFEQUEUE_HPP__