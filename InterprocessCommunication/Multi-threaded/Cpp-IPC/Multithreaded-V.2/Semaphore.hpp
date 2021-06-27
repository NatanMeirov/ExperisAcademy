#ifndef __NM_SEMAPHORE_HPP__
#define __NM_SEMAPHORE_HPP__


#include <cstddef> // size_t
#include <semaphore.h>


namespace nm {

class Semaphore {
public:
    explicit Semaphore(int a_sharedOption, unsigned int a_initialValue);
    Semaphore(const Semaphore& a_other) = delete;
    Semaphore& operator=(const Semaphore& a_other) = delete;
    ~Semaphore();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    unsigned int GetCurrentValue();
    void Down(); // Wait
    void Up(); // Post
    bool TryDown(); // TryWait
    void Destroy(); // The user must provide that before using Semaphore's Destroy (or d'tor) - NO other threads/processes are using the Semaphore!

private:
    sem_t m_semaphore;
    bool m_isAvailableSemaphore;
};

} // nm


#endif // __NM_SEMAPHORE_HPP__