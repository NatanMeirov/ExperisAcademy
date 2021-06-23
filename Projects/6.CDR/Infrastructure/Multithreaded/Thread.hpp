#ifndef __NM_THREAD_HPP__
#define __NM_THREAD_HPP__


#include <cstddef> // size_t
#include <pthread.h> // pthread_t


namespace nm {

class Thread {
public:
    typedef void* (*TaskFunction)(void* a_context);

    Thread(TaskFunction a_taskToExecute, void* a_context);
    Thread(TaskFunction a_taskToExecute);
    Thread(const Thread& a_other) = delete;
    Thread& operator=(const Thread& a_other) = delete;
    Thread(Thread&& a_rvalue) noexcept; // Move c'tor
    Thread& operator=(Thread&& a_rvalue) noexcept; // Move assignment
    ~Thread();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    void* Join(); // Returns the value that the executed function has returned after its finish
    void Detach(); // Detached thread will mark as an unavailable thread (for exit, join or detach)
    void Cancel(); // Cancels the thread (its execution will stop)

private:
    pthread_t m_threadID;
    bool* m_isAvailableThread;
};

} // nm


#endif // __NM_THREAD_HPP__