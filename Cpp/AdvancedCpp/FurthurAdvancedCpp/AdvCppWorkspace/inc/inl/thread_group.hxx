#ifndef NM_THREAD_GROUP_HXX
#define NM_THREAD_GROUP_HXX


#include <cstddef> // size_t
#include <list>
#include <memory> // std::shared_ptr
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::for_each
#include "thread.hpp"
#include "icallable.hpp"
#include "thread_group.hpp" //! REMOVE!


namespace advcpp
{

template <typename DestructionPolicy>
ThreadGroup<DestructionPolicy>::ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, const DestructionPolicy& a_destructionPolicy)
: m_threadsGroup()
, m_commonTask(a_commonTask)
, m_destructionPolicy(a_destructionPolicy)
{
    m_threadsGroup.reserve(a_threadsCount);
    for(size_t i = 0; i < a_threadsCount; ++i)
    {
        m_threadsGroup.push_back(std::shared_ptr<Thread<DestructionPolicy>>(new Thread<DestructionPolicy>(a_commonTask, a_destructionPolicy)));
    }
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
        catch(const std::exception& ex)
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
        catch(const std::exception& ex)
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
        catch(const std::exception& ex)
        {
            // Do nothing - lets the rest of the threads to detach
        }
    });
}

} // advcpp


#endif // NM_THREAD_GROUP_HXX
