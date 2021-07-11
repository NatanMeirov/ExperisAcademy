#ifndef NM_COUNTER_DECREMENT_TASK
#define NM_COUNTER_DECREMENT_TASK


#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "counter.hpp"


namespace advcpp
{

class CounterDecrementTask : public ICallable
{
public:
    CounterDecrementTask(std::shared_ptr<Counter> a_counter, const size_t a_decrementOperationsCount) : m_counter(a_counter), m_decrementOperationsCount(a_decrementOperationsCount) {}
    CounterDecrementTask(const CounterDecrementTask& a_other) = delete; // reference member
    CounterDecrementTask& operator=(const CounterDecrementTask& a_other) = delete; // reference member
    virtual ~CounterDecrementTask() = default;

    virtual void operator()() override
    {
        for(size_t i = 0; i < m_decrementOperationsCount; ++i)
        {
            DecrementCounter();
        }
    }

private:
    void DecrementCounter() { m_counter->Decrement(); }

private:
    std::shared_ptr<Counter> m_counter; // Global counter, to test the Threads on global var
    size_t m_decrementOperationsCount;
};

} // advcpp


#endif // NM_COUNTER_DECREMENT_TASK
