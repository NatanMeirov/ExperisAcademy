#ifndef NM_MUTEX_HPP
#define NM_MUTEX_HPP


#include <cstddef> // size_t
#include <pthread.h>
#include "atomic_value.hpp"


namespace advcpp
{

class Mutex
{
    friend class ConditionalVariable;
public:
    Mutex();
    Mutex(const Mutex& a_other) = delete; // Forcing to pass Mutex only by reference
    Mutex& operator=(const Mutex& a_other) = delete;
    ~Mutex();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    void Lock();
    void Unlock();
    bool TryLock(); // Returns false if mutex is not available
    void Destroy();

private:
    pthread_mutex_t& GetInnerMutex();

private:
    pthread_mutex_t m_mutex;
    AtomicFlag m_isAvailableMutex;
    AtomicFlag m_isLocked;
};

} // nm


#endif // NM_MUTEX_HPP