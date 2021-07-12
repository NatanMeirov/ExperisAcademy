#ifndef NM_THREAD_HPP
#define NM_THREAD_HPP


#include <cstddef> // size_t
#include <pthread.h> // pthread_t
#include <memory> // std:shared_ptr
#include "icallable.hpp"
#include "barrier.hpp"
#include "sync_handler.hpp"
#include "atomic_value.hpp"


namespace advcpp
{

/**
 * The ICallable object should contain its parameters inside itself, and implement operator() [returned value should be stored IN the function object]
 *
 * Concept of DestructionPolicy: policy must be copy-constructable
 * The destruction policy is a FUNCTOR (implements operator() and get 1 param: Thread& obj), to be used as an instructions to know which action the Thread
 * object should call on itself when it is in a destruction stage
 */
template <typename DestructionPolicy>
class Thread {
    friend DestructionPolicy;
public:
    Thread(std::shared_ptr<ICallable> a_task, DestructionPolicy a_policy); // DestructionPolicy is a lightweight object (can get it by value (copy))
    Thread(const Thread& a_other) = delete;
    Thread& operator=(const Thread& a_other) = delete;
    ~Thread();

    void Join();
    void Detach();
    void Cancel(bool a_ensureCompleteCancelation = false);

    bool HasDone();

private:
    static constexpr unsigned int BARRIER_COUNT = 2;
    static void* Task(void* a_ptrWrapper);
    int CallJoin();
    int CallDetach();
    int CallCancel();
    int CompleteCancel();
    int ConditionalJoin(); // For policy uses
    int ConditionalDetach(); // For policy uses
    int ConditionalCancel(bool a_ensureCompleteCancelation = false); // For policy uses

private:
    std::shared_ptr<ICallable> m_task;
    DestructionPolicy m_destructionPolicy;
    Barrier m_barrier;
    std::shared_ptr<SyncHandler> m_syncHandler;
    pthread_t m_threadID;
    AtomicFlag m_isAvailableThread;
    AtomicFlag m_hasCanceledThread;
    AtomicFlag m_hasStartedOperation;
};

} // advcpp


#include "inl/thread.hxx"


#endif // NM_THREAD_HPP
