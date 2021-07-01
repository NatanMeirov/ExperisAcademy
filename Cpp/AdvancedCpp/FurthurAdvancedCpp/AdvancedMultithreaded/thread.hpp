#ifndef NM_THREAD_HPP
#define NM_THREAD_HPP


#include <cstddef> // size_t
#include <pthread.h> // pthread_t
#include <tuple>


namespace nm
{

/**
 * Concept of Func: Func must be Call-able (function pointer, functor, lambda expression) - and must be declared to get {Args...} as its parameters, and must be copy-constructable
 * Concept of Args...: Args must be default-constructable and copy-constuctable (can be 0..N arguments)
 * Concept of RetT: RetT must be copy-constructable, and MUST NOT be of type void
 * Note: To use this Thread with member function - wrap [this] pointer inside a functor Func or send it as one of the Arg...
 */
template <typename Func, typename RetT, typename ...Args>
class Thread {
public:
    // A destruction action indicator, to be used as an indicator to know which action the Thread
    // object should call on itself when IT DOES NOT MOVE, but the destructor had called on the object
    enum DestructionAction { JOIN, DETACH, CANCEL };

    Thread(Func a_task, Args... a_args, DestructionAction a_destructionActionIndicator);
    Thread(const Thread& a_other) = delete;
    Thread& operator=(const Thread& a_other) = delete;
    Thread(Thread&& a_rvalue) noexcept; // Move c'tor
    Thread& operator=(Thread&& a_rvalue) noexcept; // Move assignment (invokes Cancel() on [this] before the move assignment operation)
    ~Thread();

    void* operator new(size_t size) = delete;
    void* operator new(size_t size, void* loc) = delete;
    void* operator new[](size_t size) = delete;
    void* operator new[](size_t size, void* loc) = delete;
    void operator delete(void* ptr) = delete;
    void operator delete[](void* ptr) = delete;

    RetT Join();
    void Detach();
    void Cancel();

private:
    static void* Task(void* a_this);

private:
    Func m_task;
    std::tuple<Args...> m_args;
    pthread_t m_threadID;
    DestructionAction m_destructionActionIndicator;
    bool m_isAvailableThread;
    bool m_isMoving;
};

} // nm


#include "inl/thread.hxx"


#endif // NM_THREAD_HPP
