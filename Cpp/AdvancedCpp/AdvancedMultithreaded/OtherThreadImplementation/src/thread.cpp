#include <cstddef> // size_t
#include <pthread.h>
#include <memory> // std:shared_ptr
#include <cassert> // assert
#include "thread.hpp"
#include "icallable.hpp"
#include "safe_ref_counting_shared_ptr_wrapper.hpp"


namespace advcpp
{

void* Thread::Task(void* a_ptrWrapper)
{
    SafeRefCountingSharedPtrWrapper<ICallable>* ptrWrapper = static_cast<SafeRefCountingSharedPtrWrapper<ICallable>*>(a_ptrWrapper);
    std::shared_ptr<ICallable> task = ptrWrapper->GetInnerSharedPtr(); // Copies the std::shared_ptr -> ++ref_count

    delete ptrWrapper; // For safety reasons (exception safe) // --ref_count (but still more then 0 -> the inner pointer is not deleted)

    (*task)();

    return nullptr;
}


Thread::Thread(std::shared_ptr<ICallable> a_task, DestructionAction a_destructionActionIndicator)
: m_task(a_task)
, m_threadID()
, m_destructionActionIndicator(a_destructionActionIndicator)
, m_isAvailableThread(true)
, m_hasMoved(false)
{
    SafeRefCountingSharedPtrWrapper<ICallable>* sharedPtrWrapper = new SafeRefCountingSharedPtrWrapper<ICallable>(m_task);
    int statusCode = pthread_create(&m_threadID, nullptr, Thread::Task, sharedPtrWrapper);
    if(statusCode < 0)
    {
        delete sharedPtrWrapper;
        throw std::runtime_error("Failed while trying to create a thread");
    }
}


Thread::Thread(Thread&& a_rvalue) noexcept
: m_task(a_rvalue.m_task)
, m_threadID(a_rvalue.m_threadID)
, m_destructionActionIndicator(a_rvalue.m_destructionActionIndicator)
, m_isAvailableThread(a_rvalue.m_isAvailableThread)
, m_hasMoved(false)
{
    a_rvalue.m_hasMoved = true; // Move indicator
}


Thread& Thread::operator=(Thread&& a_rvalue) noexcept
{
    if(m_isAvailableThread && !m_hasMoved)
    {
        try
        {
            Cancel();
        }
        catch(...)
        { // No need to handle (error would be thrown ONLY if the thread had finished already)
        }
    }

    m_task = a_rvalue.m_task;
    m_threadID = a_rvalue.m_threadID;
    m_destructionActionIndicator = a_rvalue.m_destructionActionIndicator;
    m_hasMoved = false;

    m_isAvailableThread = a_rvalue.m_isAvailableThread;
    a_rvalue.m_hasMoved = true; // Move indicator

    return *this;
}


Thread::~Thread()
{
    if(!m_hasMoved) // Would not invoke the destruction action
    {
        switch(m_destructionActionIndicator)
        {
        case JOIN:
        {
            ConditionalJoin();
            break;
        }

        case DETACH:
        {
            ConditionalDetach();
            break;
        }

        case CANCEL:
        {
            CallCancel();
            break;
        }

        case ASSERTION:
        {
            assert(!m_isAvailableThread);
            break;
        }
        }
    }
}


void Thread::Join()
{
    if(m_hasMoved)
    {
        throw std::runtime_error("Failed while tried to join a moved thread");
    }

    if(m_isAvailableThread)
    {
        int statusCode = CallJoin();
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while tried to join");
        }

        m_isAvailableThread = false;
    }
    else
    {
        throw std::runtime_error("Thread had been detached/joined/canceled already");
    }
}


void Thread::Detach()
{
    if(m_hasMoved)
    {
        throw std::runtime_error("Failed while tried to detach a moved thread");
    }

    if(m_isAvailableThread)
    {
        int statusCode = CallDetach();
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while tried to detach");
        }

        m_isAvailableThread = false;
    }
    else
    {
        throw std::runtime_error("Thread had been detached/joined/canceled already");
    }
}


void Thread::Cancel(bool a_ensureCompleteCancelation)
{
    if(m_hasMoved)
    {
        throw std::runtime_error("Failed while tried to cancel a moved thread");
    }

    // Cannot check the m_isAvailableThread flag as a condition, because there is a way that the thread is detached or joined, and they are not available, but they are cancelable...

    int statusCode = CallCancel();
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to cancel (maybe the thread had finished already)");
    }

    m_isAvailableThread = false;

    if(a_ensureCompleteCancelation)
    {
        CallJoin(); // pthread_join after cancel would provide a PTHREAD_CANCELED
    }
}


int Thread::CallJoin()
{
    return pthread_join(m_threadID, nullptr);
}


int Thread::CallDetach()
{
    return pthread_detach(m_threadID);
}


int Thread::CallCancel()
{
    return pthread_cancel(m_threadID);
}


int Thread::ConditionalJoin()
{
    return (m_isAvailableThread) ? CallJoin() : 0;
}


int Thread::ConditionalDetach()
{
    return (m_isAvailableThread) ? CallDetach() : 0;
}

} // advcpp
