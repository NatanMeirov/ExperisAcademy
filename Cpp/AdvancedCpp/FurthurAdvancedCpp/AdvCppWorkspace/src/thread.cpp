#include <cstddef> // size_t
#include <pthread.h>
#include <memory> // std:shared_ptr
#include "thread.hpp"
#include "icallable.hpp"


namespace advcpp
{

void* Thread::Task(void* a_task)
{
    ICallable* task = static_cast<ICallable*>(a_task);
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
    int statusCode = pthread_create(&m_threadID, nullptr, Thread::Task, static_cast<void*>(a_task.get()));
    if(statusCode < 0)
    {
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
        try
        {
            switch(m_destructionActionIndicator)
            {
            case JOIN:
            {
                Join();
                break;
            }

            case DETACH:
            {
                Detach();
                break;
            }

            case CANCEL:
            {
                Cancel();
                break;
            }
            }
        }
        catch(...)
        { // No need to handle (error would be thrown ONLY if the thread had finished/detached/joined already)
        }
    }
}


void Thread::Join()
{
    if(m_hasMoved)
    {
        throw std::runtime_error("Failed while trying to join moved thread");
    }

    if(m_isAvailableThread)
    {
        m_isAvailableThread = false;

        int statusCode = pthread_join(m_threadID, nullptr);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to join");
        }
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
        throw std::runtime_error("Failed while trying to detach moved thread");
    }

    if(m_isAvailableThread)
    {
        m_isAvailableThread = false;

        int statusCode = pthread_detach(m_threadID);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to detach");
        }
    }
    else
    {
        throw std::runtime_error("Thread had been detached/joined/canceled already");
    }
}


void Thread::Cancel()
{
    if(m_hasMoved)
    {
        throw std::runtime_error("Failed while trying to cancel moved thread");
    }

    m_isAvailableThread = false; // Cannot check this flag as a condition, because there is a way that the thread is detached or joined, and they are not available, but they are cancelable...

    int statusCode = pthread_cancel(m_threadID);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while trying to cancel (maybe the thread had finished already)");
    }
}

} // advcpp
