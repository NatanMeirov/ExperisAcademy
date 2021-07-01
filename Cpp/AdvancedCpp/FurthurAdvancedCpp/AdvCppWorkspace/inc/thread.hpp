#ifndef NM_THREAD_HPP
#define NM_THREAD_HPP


#include <cstddef> // size_t
#include <pthread.h> // pthread_t
#include <memory> // std:shared_ptr
#include "icallable.hpp"


namespace advcpp
{

/**
 * The ICallable object should contain its parameters inside it, and implement operator() [returned value should be stored IN the function object]
 */
class Thread {
public:

    // A destruction action indicator, to be used as an indicator to know which action the Thread
    // object should call on itself when IT DOES NOT MOVE, but the destructor had called on the object
    enum DestructionAction { JOIN, DETACH, CANCEL };

    Thread(std::shared_ptr<ICallable> a_task, DestructionAction a_destructionActionIndicator);
    Thread(const Thread& a_other) = delete;
    Thread& operator=(const Thread& a_other) = delete;
    Thread(Thread&& a_rvalue) noexcept; // Move c'tor
    Thread& operator=(Thread&& a_rvalue) noexcept; // Move assignment
    ~Thread();

    void Join();
    void Detach();
    void Cancel();

private:
    static void* Task(void* a_task);

private:
    std::shared_ptr<ICallable> m_task;
    pthread_t m_threadID;
    DestructionAction m_destructionActionIndicator;
    bool m_isAvailableThread;
    bool m_hasMoved;
};

} // advcpp


#endif // NM_THREAD_HPP
