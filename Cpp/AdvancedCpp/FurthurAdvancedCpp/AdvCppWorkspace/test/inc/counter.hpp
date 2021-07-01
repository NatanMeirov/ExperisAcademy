#ifndef NM_COUNTER_HPP
#define NM_COUNTER_HPP


#include <cstddef> // size_t
#include <iostream> // std::ostream


namespace advcpp
{

class Counter
{
    friend std::ostream& operator<<(std::ostream& a_os, const Counter& a_counter);
public:
    explicit Counter(const size_t a_countFrom = 0);
    Counter(const Counter& a_other) = default;
    Counter& operator=(const Counter& a_other) = default;
    ~Counter() = default;

    void Increment();
    void Decrement();
    size_t Count() const;

    bool operator==(const Counter& a_other) const;
    bool operator<(const Counter& a_other) const;
    bool operator>(const Counter& a_other) const;

private:
    size_t m_count;
};

} // advcpp


#endif // NM_COUNTER_HPP