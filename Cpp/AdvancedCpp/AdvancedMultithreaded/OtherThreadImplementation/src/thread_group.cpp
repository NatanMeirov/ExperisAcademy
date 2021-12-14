#include "thread_group.hpp"
#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::for_each
#include "thread.hpp"
#include "icallable.hpp"


constexpr advcpp::Thread::DestructionAction advcpp::ThreadGroup::DESTRUCT_THREAD_OPT[]; // Definition


advcpp::ThreadGroup::ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, GroupDestructionAction a_destructionActionIndicator)
: m_threadsGroup()
{
    m_threadsGroup.reserve(a_threadsCount);
    for(size_t i = 0; i < a_threadsCount; ++i)
    {
        m_threadsGroup.push_back(std::shared_ptr<Thread>(new Thread(a_commonTask, ThreadGroup::DESTRUCT_THREAD_OPT[a_destructionActionIndicator])));
    }
}


void advcpp::ThreadGroup::Join()
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [](std::shared_ptr<Thread> a_thread)
    {
        try
        {
            a_thread->Join();
        }
        catch(const std::exception& ex)
        {
            // Do nothing - lets the rest of the threads to detach
        }
    });


}


void advcpp::ThreadGroup::Detach()
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [](std::shared_ptr<Thread> a_thread)
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


void advcpp::ThreadGroup::Cancel(bool a_ensureCompleteCancelation)
{
    std::for_each(m_threadsGroup.begin(), m_threadsGroup.end(), [a_ensureCompleteCancelation](std::shared_ptr<Thread> a_thread)
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