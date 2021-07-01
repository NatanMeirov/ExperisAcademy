#ifndef NM_THREAD_HXX
#define NM_THREAD_HXX


#include <pthread.h> // pthread_create, pthread_join, pthread_exit
#include <stdexcept> // std::runtime_error
#include <tuple> // std::tuple, std::make_tuple
#include <utility> // std::move
#include "tuple_unpack.hxx"


namespace nm
{

template <typename Func, typename RetT, typename ...Args>
void* Thread<Func,RetT,Args...>::Thread::Task(void* a_this)
{
    Thread<Func,RetT,Args...>* self = static_cast<Thread<Func,RetT,Args...>*>(a_this);
    self->m_task(self->m_args);
    RetT returnedValue = nm::meta::UnpackTupleAndCallFunc<RetT>(self->m_task, self->m_args);

    return reinterpret_cast<void*>(returnedValue);
}


template <typename Func, typename RetT, typename ...Args>
Thread<Func,RetT,Args...>::Thread(Func a_task, Args... a_args, DestructionAction a_destructionActionIndicator)
: m_task(a_task)
, m_args(std::make_tuple(a_args...))
, m_threadID()
, m_destructionActionIndicator(a_destructionActionIndicator)
, m_isAvailableThread(true)
, m_isMoving(false)
{
    int statusCode = pthread_create(&m_threadID, nullptr, Thread::Task, static_cast<void*>(this));
    if(statusCode < 0)
    {
        throw std::runtime_error("Failed while trying to create a thread");
    }
}


template <typename Func, typename RetT, typename ...Args>
Thread<Func,RetT,Args...>::Thread(Thread&& a_rvalue) noexcept
: m_task(a_rvalue.m_task)
, m_args(std::move(a_rvalue.m_args))
, m_threadID(a_rvalue.m_threadID)
, m_destructionActionIndicator(a_rvalue.m_destructionActionIndicator)
, m_isAvailableThread(a_rvalue.m_isAvailableThread)
, m_isMoving(false)
{
    a_rvalue.m_isMoving = true; // Move indicator
}


template <typename Func, typename RetT, typename ...Args>
Thread<Func,RetT,Args...>& Thread<Func,RetT,Args...>::operator=(Thread&& a_rvalue) noexcept
{
    if(m_isAvailableThread)
    {
        try
        {
            Cancel();
        }
        catch(...)
        { // No need to handle (error would be thrown ONLY if the thread has finished already)
        }
    }

    m_task = a_rvalue.m_task;
    m_args = std::move(a_rvalue.m_args);
    m_threadID = a_rvalue.m_threadID;
    m_destructionActionIndicator = a_rvalue.m_destructionActionIndicator;
    m_isMoving = false;

    m_isAvailableThread = a_rvalue.m_isAvailableThread;
    a_rvalue.m_isMoving = true; // Move indicator

    return *this;
}


template <typename Func, typename RetT, typename ...Args>
Thread<Func,RetT,Args...>::~Thread()
{
    if(!m_isMoving) // Would not invoke the destruction action
    {
        switch(m_destructionActionIndicator)
        {
        case JOIN:
        {
            Join(); // Return value is ignored
        }

        case DETACH:
        {
            Detach();
        }

        case CANCEL:
        {
            try
            {
                Cancel();
            }
            catch (...)
            { // No need to handle (error would be thrown ONLY if the thread has finished already)
            }
        }
        }
    }
}


template <typename Func, typename RetT, typename ...Args>
RetT Thread<Func,RetT,Args...>::Join()
{
    void* taskReturnedValue = nullptr;
    if(m_isAvailableThread)
    {
        int statusCode = pthread_join(m_threadID, &taskReturnedValue);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to join");
        }

        m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
    }

    return reinterpret_cast<RetT>(taskReturnedValue);
}


template <typename Func, typename RetT, typename ...Args>
void Thread<Func,RetT,Args...>::Detach()
{
    if(m_isAvailableThread)
    {
        int statusCode = pthread_detach(m_threadID);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to detach");
        }

        m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
    }
}


template <typename Func, typename RetT, typename ...Args>
void Thread<Func,RetT,Args...>::Cancel()
{
    int statusCode = pthread_cancel(m_threadID);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while trying to cancel (maybe thread had finished already)");
    }

    m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
}

} // nm


#endif // NM_THREAD_HXX
