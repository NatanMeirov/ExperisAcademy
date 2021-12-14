#include "counter.hpp"
#include "atomic_value.hpp"


advcpp::Counter::Counter(size_t a_countFrom)
: m_count(a_countFrom)
{
}


void advcpp::Counter::Increment()
{
    ++m_count;
}


void advcpp::Counter::Decrement()
{
    --m_count;
}


size_t advcpp::Counter::Count() const
{
    return m_count;
}
