#ifndef NM_THREAD_GROUP_HXX
#define NM_THREAD_GROUP_HXX


#include <cstddef> // size_t
#include <list>
#include <memory> // std::shared_ptr
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::for_each
#include "thread.hpp"
#include "icallable.hpp"


namespace advcpp
{

template <typename DestructionPolicy>
ThreadGroup<DestructionPolicy>::ThreadGroup(std::shared_ptr<ICallable> a_commonTask, size_t a_threadsCount, DestructionPolicy a_destructionPolicy)
: m_threadsGroup()
, m_commonTask(a_commonTask)
, m_size(0)
, m_destructionPolicy(a_destructionPolicy)
{
    AddThreads(a_threadsCount);
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::Join()
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [](std::shared_ptr<Thread<DestructionPolicy>> a_thread)
    {
        try
        {
            a_thread->Join();
        }
        catch(...)
        {
            // Do nothing - let the rest of the threads detach
        }
    });
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::Detach()
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [](std::shared_ptr<Thread<DestructionPolicy>> a_thread)
    {
        try
        {
            a_thread->Detach();
        }
        catch(...)
        {
            // Do nothing - lets the rest of the threads to detach
        }
    });
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::Cancel(bool a_ensureCompleteCancelation)
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [a_ensureCompleteCancelation](std::shared_ptr<Thread<DestructionPolicy>> a_thread)
    {
        try
        {
            a_thread->Cancel(a_ensureCompleteCancelation);
        }
        catch(...)
        {
            // Do nothing - lets the rest of the threads to detach
        }
    });
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::Add(size_t a_threadsToAdd)
{
    CleanDoneThreads();
    AddThreads(a_threadsToAdd);
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::Remove(size_t a_threadsToRemove)
{
    size_t cleanedThreads = CleanDoneThreads();
    if(cleanedThreads >= a_threadsToRemove)
    {
        return;
    }

    KillThreads(a_threadsToRemove - cleanedThreads);
}


template <typename DestructionPolicy>
size_t ThreadGroup<DestructionPolicy>::Size() const
{
    return m_size.Get();
}


template <typename DestructionPolicy>
size_t ThreadGroup<DestructionPolicy>::CleanDoneThreads()
{
    size_t removedThreadsCount = 0;
    auto itr = m_threadsGroup.begin();
    auto endItr = m_threadsGroup.end();

    while(itr != endItr)
    {
        if((*itr)->HasDone())
        {
            itr = m_threadsGroup.erase(itr);
            ++removedThreadsCount;
            --m_size;
        }
        else
        {
            ++itr;
        }
    }

    return removedThreadsCount;
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::AddThreads(size_t a_threadsToAdd)
{
    for(size_t i = 0; i < a_threadsToAdd; ++i)
    {
        m_threadsGroup.push_back(std::shared_ptr<Thread<DestructionPolicy>>(new Thread<DestructionPolicy>(m_commonTask, m_destructionPolicy)));
        ++m_size;
    }
}


template <typename DestructionPolicy>
void ThreadGroup<DestructionPolicy>::KillThreads(size_t a_threadsToKill)
{
    size_t remainedThreads = m_size.Get();
    for(size_t i = 0; i < a_threadsToKill && i < remainedThreads; ++i)
    {
        try
        {
            m_threadsGroup.back()->Cancel();
        }
        catch (...)
        {
            // Do nothing - the thread might had finished already
        }

        m_threadsGroup.pop_back();
        --m_size;
    }
}

} // advcpp


#endif // NM_THREAD_GROUP_HXX
