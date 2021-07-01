#include "counter.hpp"


advcpp::Counter::Counter(const size_t a_countFrom)
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

bool advcpp::Counter::operator==(const Counter& a_other) const
{
    return m_count == a_other.m_count;
}


bool advcpp::Counter::operator<(const Counter& a_other) const
{
    return m_count < a_other.m_count;
}


bool advcpp::Counter::operator>(const Counter& a_other) const
{
    return m_count > a_other.m_count;
}