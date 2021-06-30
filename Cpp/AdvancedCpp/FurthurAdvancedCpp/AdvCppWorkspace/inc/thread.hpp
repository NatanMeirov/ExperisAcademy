#ifndef NM_THREAD_HPP
#define NM_THREAD_HPP


#include <cstddef> // size_t
#include <pthread.h> // pthread_t
#include <tuple>


namespace nm
{

namespace advcpp
{

/**
 * Concept of Func: Func must be Call-able (function pointer, functor, lambda expression) - and must be declared to get {Args...} as its parameters, and must be copy-constructable
 * Concept of Args: Args must be default-constructable and copy-constuctable (can be 0..N arguments)
 * To use this Thread with member function: wrap this pointer inside a functor Func
 */


template <typename Func, typename ...Args>
class Thread {
public:
    Thread(Func a_task, Args... a_args);
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

    void Join();
    void Detach();
    void Cancel();

private:
    static void* Task(void* a_this);

    Func m_task;
    std::tuple<Args...> m_args;
    bool* m_isAvailableThread;
    pthread_t m_threadID;
};


} // advcpp

} // nm


#include "inl/thread.hxx"


#endif // NM_THREAD_HPP
