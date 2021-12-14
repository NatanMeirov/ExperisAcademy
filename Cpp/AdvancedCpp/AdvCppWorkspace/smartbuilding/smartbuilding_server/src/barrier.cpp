#include "barrier.hpp"
#include <pthread.h> // POSIX pthread's barrier function
#include <stdexcept> // std::invalid_argument, std::runtime_error


advcpp::Barrier::Barrier(size_t a_count)
: m_barrier()
{
    Initialize(a_count);
}


advcpp::Barrier::~Barrier()
{
    Destroy();
}


void advcpp::Barrier::Wait()
{
    pthread_barrier_wait(&m_barrier); // An error cannot occur, because the m_barrier is pre-initialized successfully
}


void advcpp::Barrier::Reset(size_t a_newCount)
{
    Destroy();
    Initialize(a_newCount);
}


void advcpp::Barrier::Initialize(size_t a_count)
{
    if(a_count == 0)
    {
        throw std::invalid_argument("Count must be greater than 0");
    }

    int statusCode = pthread_barrier_init(&m_barrier, nullptr, a_count);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to create a barrier");
    }
}


void advcpp::Barrier::Destroy()
{
    pthread_barrier_destroy(&m_barrier);
}
