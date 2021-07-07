#ifndef NM_SEMAPHORE_HPP
#define NM_SEMAPHORE_HPP


#include <cstddef> // size_t
#include <semaphore.h>
#include "atomic_value.hpp"


namespace advcpp
{

class Semaphore
{
public:
    Semaphore(unsigned int a_initialValue, int a_sharedOption = 0);
    Semaphore(const Semaphore& a_other) = delete;
    Semaphore& operator=(const Semaphore& a_other) = delete;
    ~Semaphore();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    int GetCurrentValue() const;
    void Down(); // Wait
    void Up(); // Post
    bool TryDown(); // TryWait
    void Destroy(); // The user must provide that before using Semaphore's Destroy (or d'tor) - NO other threads/processes are using the Semaphore!

private:
    mutable sem_t m_semaphore;
    AtomicFlag m_isAvailableSemaphore;
};

} // advcpp


#endif // NM_SEMAPHORE_HPP