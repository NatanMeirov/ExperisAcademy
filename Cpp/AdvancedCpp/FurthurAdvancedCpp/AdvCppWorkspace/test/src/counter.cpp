#include "counter.hpp"


nm::advcpp::Counter::Counter(const size_t a_countFrom)
: m_count(a_countFrom)
{
}


void nm::advcpp::Counter::Increment()
{
    ++m_count;
}


void nm::advcpp::Counter::Decrement()
{
    --m_count;
}


size_t nm::advcpp::Counter::Count() const
{
    return m_count;
}

bool nm::advcpp::Counter::operator==(const Counter& a_other) const
{
    return m_count == a_other.m_count;
}


bool nm::advcpp::Counter::operator<(const Counter& a_other) const
{
    return m_count < a_other.m_count;
}


bool nm::advcpp::Counter::operator>(const Counter& a_other) const
{
    return m_count > a_other.m_count;
}