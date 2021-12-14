#ifndef NM_COUNTER_INCREMENT_TASK
#define NM_COUNTER_INCREMENT_TASK


#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "counter.hpp"


namespace advcpp
{

class CounterIncrementTask : public ICallable
{
public:
    CounterIncrementTask(std::shared_ptr<Counter> a_counter, const size_t a_incrementOperationsCount) : m_counter(a_counter), m_incrementOperationsCount(a_incrementOperationsCount) {}
    CounterIncrementTask(const CounterIncrementTask& a_other) = delete; // reference member
    CounterIncrementTask& operator=(const CounterIncrementTask& a_other) = delete; // reference member
    virtual ~CounterIncrementTask() = default;

    virtual void operator()() override
    {
        for(size_t i = 0; i < m_incrementOperationsCount; ++i)
        {
            IncrementCounter();
        }
    }

private:
    void IncrementCounter() { m_counter->Increment(); }

private:
    std::shared_ptr<Counter> m_counter; // Global counter, to test the Threads on global var
    size_t m_incrementOperationsCount;
};

} // advcpp


#endif // NM_COUNTER_INCREMENT_TASK
