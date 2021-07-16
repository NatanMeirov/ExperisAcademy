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
    Barrier(size_t a_count);
    Barrier(const Barrier& a_other) = delete;
    Barrier& operator=(const Barrier& a_other) = delete;
    ~Barrier();

    void Wait();
    void Reset(size_t a_newCount);

private:
    void Initialize(size_t a_count);
    void Destroy();

private:
    pthread_barrier_t m_barrier;
};

} // advcpp


#endif // NM_BARRIER_HPP