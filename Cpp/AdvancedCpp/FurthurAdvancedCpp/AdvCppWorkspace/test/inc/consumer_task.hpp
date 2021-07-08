#ifndef NM_CONSUMER_TASK_HPP
#define NM_CONSUMER_TASK_HPP


#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include <vector>
#include <mutex>
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "icallable.hpp"


namespace advcpp
{

class ConsumerTask : public ICallable
{
public:
    ConsumerTask(std::shared_ptr<BlockingBoundedQueue<int,AssertPolicy<int>>> a_queue, size_t a_maximumNumberOfIterations, std::shared_ptr<std::vector<int>> a_resultVector)
    : m_queue(a_queue)
    , m_resultVector(a_resultVector)
    , m_maximumNumberOfIterations(a_maximumNumberOfIterations)
    {
    }

    virtual void operator()() override
    {
        for(size_t i = 0; i < m_maximumNumberOfIterations; ++i)
        {
            std::lock_guard<std::mutex> guard(m_lock); // To make a thread-safety push into the vector
            m_resultVector->push_back(m_queue->Dequeue());
        }
    }

private:
    std::shared_ptr<BlockingBoundedQueue<int,AssertPolicy<int>>> m_queue;
    std::shared_ptr<std::vector<int>> m_resultVector;
    std::mutex m_lock;
    size_t m_maximumNumberOfIterations;
};

} // advcpp


#endif // NM_CONSUMER_TASK_HPP