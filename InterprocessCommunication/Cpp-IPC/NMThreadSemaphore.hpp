#ifndef __NM_NMThreadSemaphore_HPP__
#define __NM_NMThreadSemaphore_HPP__


#include <cstddef> // size_t
#include <pthread.h>


namespace nm {

// Self implementation of a (threads) Semaphore using mutex and conditional variable
class NMThreadSemaphore {
public:
    explicit NMThreadSemaphore(unsigned int a_count);
    NMThreadSemaphore(const NMThreadSemaphore& a_other) = delete;
    NMThreadSemaphore& operator=(const NMThreadSemaphore& a_other) = delete;
    ~NMThreadSemaphore(); // The user must provide that before using Semaphore's Destroy - NO other threads/processes are using the Semaphore!

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    unsigned int GetCurrentCount() const;
    void Down(); // Wait
    void Up(); // Post

private:
    pthread_mutex_t m_mutexLock;
    pthread_cond_t m_conditionalVariable;
    unsigned int m_count;
};

} // nm


#endif // __NM_NMThreadSemaphore_HPP__
