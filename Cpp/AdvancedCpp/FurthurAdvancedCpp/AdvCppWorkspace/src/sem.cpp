#include "sem.hpp"
#include <semaphore.h> // semaphore functions
#include <stdexcept> // std::runtime_error
#include <cassert> // assert
#include "atomic_value.hpp"


advcpp::Semaphore::Semaphore(int a_sharedOption, unsigned int a_initialValue)
: m_semaphore()
, m_isAvailableSemaphore(true)
{
    int statusCode = sem_init(&m_semaphore, a_sharedOption, a_initialValue);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while trying to initialize the Semaphore");
    }
}


advcpp::Semaphore::~Semaphore()
{
    Destroy();
}


int advcpp::Semaphore::GetCurrentValue() const
{
    if(m_isAvailableSemaphore.Check())
    {
        int currentValue;
        sem_getvalue(&m_semaphore, &currentValue);

        return currentValue;
    }

    return 0;
}


void advcpp::Semaphore::Up()
{
    if(m_isAvailableSemaphore.Check())
    {
        int statusCode = sem_post(&m_semaphore);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to post");
        }
    }
}


void advcpp::Semaphore::Down() {
    if(m_isAvailableSemaphore.Check())
    {
        int statusCode = sem_wait(&m_semaphore);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while trying to wait");
        }
    }
}


bool advcpp::Semaphore::TryDown()
{
    if(m_isAvailableSemaphore.Check())
    {
        return (sem_trywait(&m_semaphore) == 0) ? true : false;
    }

    return false;
}


void advcpp::Semaphore::Destroy()
{
    if(m_isAvailableSemaphore.Check())
    {
        assert(sem_destroy(&m_semaphore) == 0);
        m_isAvailableSemaphore.False();
    }
}