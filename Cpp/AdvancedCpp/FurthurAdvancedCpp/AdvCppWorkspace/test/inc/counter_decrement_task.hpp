#ifndef NM_COUNTER_DECREMENT_TASK
#define NM_COUNTER_DECREMENT_TASK


#include "icallable.hpp"
#include "counter.hpp"


namespace advcpp
{

class CounterDecrementTask : public ICallable
{
public:
    CounterDecrementTask(Counter& a_counter, const size_t a_decrementOperationsCount) : m_counter(a_counter), m_decrementOperationsCount(a_decrementOperationsCount) {}
    CounterDecrementTask(const CounterDecrementTask& a_other) = delete; // reference member
    CounterDecrementTask& operator=(const CounterDecrementTask& a_other) = delete; // reference member
    virtual ~CounterDecrementTask() = default;

    virtual void operator()() override { for(size_t i = 0; i < m_decrementOperationsCount; ++i) { DecrementCounter();} }

private:
    void DecrementCounter() { m_counter.Decrement(); }

private:
    Counter& m_counter; // Global counter, to test the Threads on global var
    size_t m_decrementOperationsCount;
};

} // advcpp


#endif // NM_COUNTER_DECREMENT_TASK
