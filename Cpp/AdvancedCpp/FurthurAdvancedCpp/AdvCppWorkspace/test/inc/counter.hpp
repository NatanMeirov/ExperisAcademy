#ifndef NM_COUNTER_HPP
#define NM_COUNTER_HPP


#include <cstddef> // size_t
#include <iostream> // std::ostream
#include "atomic_value.hpp"


namespace advcpp
{

class Counter
{
    friend std::ostream& operator<<(std::ostream& a_os, const Counter& a_counter);
public:
    explicit Counter(size_t a_countFrom = 0);
    Counter(const Counter& a_other) = default;
    Counter& operator=(const Counter& a_other) = default;
    ~Counter() = default;

    void Increment();
    void Decrement();
    size_t Count() const;

private:
    AtomicValue<size_t> m_count;
};

} // advcpp


#endif // NM_COUNTER_HPP