#ifndef NM_WORKS_ENQUEUER_HPP
#define NM_WORKS_ENQUEUER_HPP


#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


namespace advcpp
{


// Concept of QueueTypeDestructionPolicy: must be a destruction policy of the given Queue type, and must be a destruction policy of type T = std::shared_ptr<ICallable>
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueType: QueueType must implement Enqueue, Dequeue, IsEmpty and Size methods (Suggestion: these methods should be multithreaded-safe!),
// and its T MUST be std::shared_ptr of type ICallable (QueueType< T = std::shared_ptr<ICallable> >),
// and it must implement a C'tor of: {size_t, QueueTypeDestructionPolicy<std::shared_ptr<ICallable>>}
template <typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class WorksEnqueuer : public ICallable
{
    using Work = std::shared_ptr<ICallable>;
public:
    WorksEnqueuer(std::shared_ptr<QueueType> a_worksQueue, Work a_workToEnqueue);
    WorksEnqueuer(const WorksEnqueuer& a_other) = delete;
    WorksEnqueuer& operator=(const WorksEnqueuer& a_other) = delete;
    ~WorksEnqueuer() = default;

    virtual void operator()() override;

private:
    std::shared_ptr<QueueType> m_worksQueue;
    Work m_workToEnqueue;
};

} // advcpp


#include "inl/works_enqueuer.hxx"


#endif // NM_WORKS_ENQUEUER_HPP