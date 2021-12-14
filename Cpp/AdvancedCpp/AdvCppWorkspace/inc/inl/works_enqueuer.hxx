#ifndef NM_WORKS_ENQUEUER_HXX
#define NM_WORKS_ENQUEUER_HXX

#include <memory> // std::shared_ptr
#include "icallable.hpp"


namespace advcpp
{

template <typename QueueTypeDestructionPolicy, typename QueueType>
WorksEnqueuer<QueueTypeDestructionPolicy,QueueType>::WorksEnqueuer(std::shared_ptr<QueueType> a_worksQueue, Work a_workToEnqueue)
: m_worksQueue(a_worksQueue)
, m_workToEnqueue(a_workToEnqueue)
{
}


template <typename QueueTypeDestructionPolicy, typename QueueType>
void WorksEnqueuer<QueueTypeDestructionPolicy,QueueType>::operator()()
{
    try
    {
        m_worksQueue->Enqueue(m_workToEnqueue);
    }
    catch(...)
    {
        // Do nothing
    }
}

} // advcpp


#endif // NM_WORKS_ENQUEUER_HXX