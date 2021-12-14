#ifndef NM_PRODUCER_TASK_HPP
#define NM_PRODUCER_TASK_HPP


#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "icallable.hpp"


namespace advcpp
{

class ProducerTask : public ICallable
{
public:
    ProducerTask(std::shared_ptr<BlockingBoundedQueue<int,AssertPolicy<int>>> a_queue, size_t a_maximumNumberOfIterations)
    : m_queue(a_queue)
    , m_maximumNumberOfIterations(a_maximumNumberOfIterations)
    {
    }

    virtual void operator()() override
    {
        for(size_t i = 0; i < m_maximumNumberOfIterations; ++i)
        {
            m_queue->Enqueue(i);
        }
    }

private:
    std::shared_ptr<BlockingBoundedQueue<int,AssertPolicy<int>>> m_queue;
    size_t m_maximumNumberOfIterations;
};

} // advcpp


#endif // NM_PRODUCER_TASK_HPP