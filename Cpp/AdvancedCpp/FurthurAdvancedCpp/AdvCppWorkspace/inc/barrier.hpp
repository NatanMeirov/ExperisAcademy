#ifndef NM_BARRIER_HPP
#define NM_BARRIER_HPP


#include <cstddef> // size_t
#include <pthread.h>


namespace advcpp
{

// Count MUST be greater than 0
class Barrier
{
public:
    Barrier(const size_t a_count);
    Barrier(const Barrier& a_other) = delete;
    Barrier& operator=(const Barrier& a_other) = delete;
    ~Barrier();

    void Wait();

private:
    pthread_barrier_t m_barrier;
};

} // advcpp


#endif // NM_BARRIER_HPP