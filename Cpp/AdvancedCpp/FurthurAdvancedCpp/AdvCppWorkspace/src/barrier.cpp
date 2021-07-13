#include "barrier.hpp"
#include <pthread.h> // barrier function
#include <stdexcept> // std::invalid_argument, std::runtime_error


advcpp::Barrier::Barrier(const size_t a_count)
: m_barrier()
{
    if(!a_count)
    {
        throw std::invalid_argument("Count must be greater than 0");
    }

    int statusCode = pthread_barrier_init(&m_barrier, nullptr, a_count);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to create a barrier");
    }
}


advcpp::Barrier::~Barrier()
{
    pthread_barrier_destroy(&m_barrier);
}


void advcpp::Barrier::Wait()
{
    pthread_barrier_wait(&m_barrier); // An error cannot occur, because the m_barrier is pre-initialized successfully
}
