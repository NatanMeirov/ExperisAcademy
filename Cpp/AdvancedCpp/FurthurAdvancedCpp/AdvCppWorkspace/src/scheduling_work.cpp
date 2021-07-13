#include "scheduling_work.hpp"
#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "atomic_value.hpp"
#include "sync_handler.hpp"


advcpp::SchedulingWork::SchedulingWork(WorksQueue a_worksQueue, std::shared_ptr<SyncHandler> a_syncHandler)
: m_worksQueue(a_worksQueue)
, m_syncHandler(a_syncHandler)
{
}


void advcpp::SchedulingWork::operator()()
{
    while(!m_syncHandler->Check()) // While there isn't a signal that the current task should stop
    {
        std::shared_ptr<ICallable> work;
        m_worksQueue->Dequeue(work);
        (*work)();
    }

    m_hasFinished.True();
}
