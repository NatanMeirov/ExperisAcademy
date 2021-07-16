#ifndef NM_THREAD_HXX
#define NM_THREAD_HXX


#include <cstddef> // size_t
#include <memory> // std:shared_ptr
#include <exception> // std::current_exception
#include <pthread.h>
#include <cxxabi.h> // abi::__forced_unwind
#include "icallable.hpp"


namespace advcpp
{

template <typename DestructionPolicy>
void* Thread<DestructionPolicy>::Task(void* a_this)
{
    Thread<DestructionPolicy>* self = static_cast<Thread<DestructionPolicy>*>(a_this);

    std::shared_ptr<ICallable> task = self->m_task; // task: ++ref_count
    std::shared_ptr<SyncHandler> syncHandler = self->m_syncHandler; // syncHandler: ++ref_count (now even if the thread destroys - the SyncHandler would not be destroyed)

    self->m_barrier.Wait(); // The Thread's c'tor can done by now (we already saved the shared_ptrs' ref count, and it should be atleast 1 [both])

    // Exception safety:
    try
    {
        (*task)();
    }
    catch(const abi::__forced_unwind& ex)
    {
        syncHandler->Signal(); // Post
        throw; // MUST rethrow the stack unwinding handler exception - to handle the destruction of the thread's stack variables on a cancelation
    }
    catch(...)
    {
        syncHandler->SetException(std::current_exception());
    }

    syncHandler->Signal(); // Post

    return nullptr;
}


template <typename DestructionPolicy>
Thread<DestructionPolicy>::Thread(std::shared_ptr<ICallable> a_task, DestructionPolicy a_destructionPolicy)
: m_task(a_task)
, m_destructionPolicy(a_destructionPolicy)
, m_barrier(Thread::BARRIER_COUNT)
, m_syncHandler(new SyncHandler())
, m_threadID()
, m_isAvailableThread(true)
, m_hasCanceledThread(false)
, m_hasStartedOperation(false)
{
    int statusCode = pthread_create(&m_threadID, nullptr, Thread::Task, this);
    if(statusCode < 0)
    {
        throw std::runtime_error("Failed while tried to create a thread");
    }

    m_barrier.Wait(); // To handle the race condition (the Thread Object will not be destructed that way, and will still be alive even if it should be destructed before the execution of Task)
}


template <typename DestructionPolicy>
Thread<DestructionPolicy>::~Thread()
{
    m_destructionPolicy(*this);
}


template <typename DestructionPolicy>
void Thread<DestructionPolicy>::Join()
{
    if(m_isAvailableThread.Check() && !m_hasCanceledThread.Check())
    {
        if(m_hasStartedOperation.SetIf(false, true))
        {
            int statusCode = CallJoin();
            if(statusCode != 0)
            {
                throw std::runtime_error("Failed while tried to join a thread");
            }

            m_isAvailableThread.False();
            m_hasStartedOperation.SetIf(true, false);
        }
    }
    else
    {
        throw std::runtime_error("Thread had been detached/joined/canceled already");
    }
}


template <typename DestructionPolicy>
void Thread<DestructionPolicy>::Detach()
{
    if(m_isAvailableThread.Check() && !m_hasCanceledThread.Check())
    {
        if(m_hasStartedOperation.SetIf(false, true))
        {
            int statusCode = CallDetach();
            if(statusCode != 0)
            {
                throw std::runtime_error("Failed while tried to detach a thread");
            }

            m_isAvailableThread.False();
            m_hasStartedOperation.SetIf(true, false);
        }
    }
    else
    {
        throw std::runtime_error("Thread had been detached/joined/canceled already");
    }
}


template <typename DestructionPolicy>
void Thread<DestructionPolicy>::Cancel(bool a_ensureCompleteCancelation)
{
    int statusCode = CallCancel();
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to cancel a thread (maybe the thread had finished already)");
    }

    m_hasCanceledThread.True();

    if(a_ensureCompleteCancelation)
    {
        CallJoin(); // pthread_join after cancel would provide a PTHREAD_CANCELED
    }
}


template <typename DestructionPolicy>
bool Thread<DestructionPolicy>::HasDone()
{
    return m_syncHandler->Check();
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::CallJoin()
{
    return pthread_join(m_threadID, nullptr);
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::CallDetach()
{
    return pthread_detach(m_threadID);
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::CallCancel()
{
    return pthread_cancel(m_threadID);
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::ConditionalJoin()
{
    return (m_isAvailableThread.Check()) ? CallJoin() : 0;
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::ConditionalDetach()
{
    return (m_isAvailableThread.Check()) ? CallDetach() : 0;
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::ConditionalCancel(bool a_ensureCompleteCancelation)
{
    if(!m_hasCanceledThread.Check())
    {
        return (a_ensureCompleteCancelation) ? CompleteCancel() : CallCancel();
    }

    return 0;
}


template <typename DestructionPolicy>
int Thread<DestructionPolicy>::CompleteCancel()
{
    int cancelStatus = CallCancel();
    int joinStatus = CallJoin();

    return cancelStatus && joinStatus;
}

} // advcpp


#endif // NM_THREAD_HXX
