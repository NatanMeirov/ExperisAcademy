#include "../inc/thread_group.hpp"
#include "../inc/thread.hpp"
#include "../inc/icallable.hpp"
#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr


advcpp::ThreadGroup::ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, GroupDestructionAction a_destructionActionIndicator)
: m_threadsGroup(a_threadsCount)
, m_isAvailableThreadGroup(true)
{
    for(size_t i = 0; i < a_threadsCount; ++i)
    {
        m_threadsGroup.push_back(std::shared_ptr<Thread>(new Thread(a_commonTask, ThreadGroup::DESTRUCT_THREAD_OPT[a_destructionActionIndicator])));
    }
}


void advcpp::ThreadGroup::Join()
{
// if not available - throw
}


void advcpp::ThreadGroup::Detach()
{

// if not available - throw
}


void advcpp::ThreadGroup::Cancel()
{
    // if not available - throw
}