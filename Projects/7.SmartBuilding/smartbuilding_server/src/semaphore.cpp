#include "semaphore.hpp"
#include <semaphore.h> // semaphore functions
#include <stdexcept> // std::runtime_error
#include "atomic_value.hpp"


advcpp::Semaphore::Semaphore(unsigned int a_initialValue, int a_sharedOption)
: m_semaphore()
{
    int statusCode = sem_init(&m_semaphore, a_sharedOption, a_initialValue);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to initialize a semaphore");
    }
}


advcpp::Semaphore::~Semaphore()
{
    sem_destroy(&m_semaphore);
}


int advcpp::Semaphore::GetCurrentValue() const
{
    int currentValue;
    sem_getvalue(&m_semaphore, &currentValue);

    return currentValue;
}


void advcpp::Semaphore::Up()
{
    int statusCode = sem_post(&m_semaphore);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to post on semaphore");
    }
}


void advcpp::Semaphore::Down() {
    int statusCode = sem_wait(&m_semaphore);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to wait on semaphore");
    }
}


bool advcpp::Semaphore::TryDown()
{
    return (sem_trywait(&m_semaphore) == 0) ? true : false;
}
