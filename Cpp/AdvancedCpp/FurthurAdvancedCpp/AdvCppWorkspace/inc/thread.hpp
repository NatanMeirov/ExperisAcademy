#ifndef NM_THREAD_HPP
#define NM_THREAD_HPP


#include <cstddef> // size_t
#include <pthread.h> // pthread_t
#include <memory> // std:shared_ptr
#include "icallable.hpp"


namespace advcpp
{

/**
 * The ICallable object should contain its parameters inside itself, and implement operator() [returned value should be stored IN the function object]
 */
class Thread {
public:

    // A destruction action indicator, to be used as an indicator to know which action the Thread
    // object should call on itself when IT DOES NOT MOVE, but the destructor had called on the object
    enum DestructionAction { JOIN, DETACH, CANCEL, ASSERTION };

    Thread(std::shared_ptr<ICallable> a_task, DestructionAction a_destructionActionIndicator = ASSERTION);
    Thread(const Thread& a_other) = delete;
    Thread& operator=(const Thread& a_other) = delete;
    Thread(Thread&& a_rvalue) noexcept; // Move c'tor
    Thread& operator=(Thread&& a_rvalue) noexcept; // Move assignment
    ~Thread();

    void Join();
    void Detach();
    void Cancel(bool a_ensureCompleteCancelation = false);

private:
    static void* Task(void* a_ptrWrapper);
    int CallJoin();
    int CallDetach();
    int CallCancel();

private:
    std::shared_ptr<ICallable> m_task;
    pthread_t m_threadID;
    DestructionAction m_destructionActionIndicator;
    bool m_isAvailableThread;
    bool m_hasMoved;
};

} // advcpp


#endif // NM_THREAD_HPP
