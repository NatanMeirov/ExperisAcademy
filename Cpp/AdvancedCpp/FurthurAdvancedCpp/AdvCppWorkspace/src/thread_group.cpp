#include "thread_group.hpp"
#include "thread.hpp"
#include "icallable.hpp"
#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include <stdexcept> // std::runtime_error


constexpr advcpp::Thread::DestructionAction advcpp::ThreadGroup::DESTRUCT_THREAD_OPT[]; // Definition


advcpp::ThreadGroup::ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, GroupDestructionAction a_destructionActionIndicator)
: m_threadsGroup()
, m_isAvailableThreadGroup(true)
{
    m_threadsGroup.reserve(a_threadsCount);
    for(size_t i = 0; i < a_threadsCount; ++i)
    {
        m_threadsGroup.push_back(std::shared_ptr<Thread>(new Thread(a_commonTask, ThreadGroup::DESTRUCT_THREAD_OPT[a_destructionActionIndicator])));
    }
}


advcpp::ThreadGroup::~ThreadGroup()
{
    m_isAvailableThreadGroup = false;
}


void advcpp::ThreadGroup::Join()
{
    if(m_isAvailableThreadGroup)
    {
        m_isAvailableThreadGroup = false;

        for(size_t i = 0; i < m_threadsGroup.size(); ++i)
        {
            m_threadsGroup[i]->Join();
        }
    }
    else
    {
        throw std::runtime_error("Double join/detach.cancel attempts on a threads group");
    }
}


void advcpp::ThreadGroup::Detach()
{
    if(m_isAvailableThreadGroup)
    {
        m_isAvailableThreadGroup = false;

        for(size_t i = 0; i < m_threadsGroup.size(); ++i)
        {
            m_threadsGroup.at(i)->Detach();
        }
    }
    else
    {
        throw std::runtime_error("Double join/detach.cancel attempts on a threads group");
    }
}


void advcpp::ThreadGroup::Cancel()
{
    m_isAvailableThreadGroup = false; // flag could be false before this assignment, if the group had previously joined or detached

    for(size_t i = 0; i < m_threadsGroup.size(); ++i)
    {
        try
        {
            m_threadsGroup.at(i)->Cancel();
        }
        catch (...)
        { // The thread may finished already its execution - and a cancel would fail and an exception may be thrown (although its valid case)
        }
    }
}