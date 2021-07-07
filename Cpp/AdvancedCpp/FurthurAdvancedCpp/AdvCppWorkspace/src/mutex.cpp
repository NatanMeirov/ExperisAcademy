#include "mutex.hpp"
#include <pthread.h> // Mutex functions and MACROS
#include <stdexcept> // std::runtime_error
#include <cassert> // assert
#include "atomic_value.hpp"


advcpp::Mutex::Mutex()
: m_mutex()
, m_isAvailableMutex(true)
, m_isLocked(false)
{
    int statusCode = pthread_mutex_init(&m_mutex, nullptr);
    if(statusCode != 0)
    {
        throw std::runtime_error("Failed while tried to create a mutex");
    }
}


advcpp::Mutex::~Mutex()
{
    Destroy();
}


void advcpp::Mutex::Lock()
{
    if(m_isAvailableMutex.Check())
    {
        int statusCode = pthread_mutex_lock(&m_mutex);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while tried to lock the mutex");
        }

        m_isLocked.True();
    }
}


void advcpp::Mutex::Unlock()
{
    if(m_isAvailableMutex.Check())
    {
        if(!m_isLocked.Check())
        {
            throw std::runtime_error("Mutex has already unlocked");
        }

        int statusCode = pthread_mutex_unlock(&m_mutex);
        if(statusCode != 0)
        {
            throw std::runtime_error("Failed while tried to unlock the mutex");
        }

        m_isLocked.False();
    }
}


bool advcpp::Mutex::TryLock()
{
    if(m_isAvailableMutex.Check())
    {
        return (pthread_mutex_trylock(&m_mutex) == 0) ? true : false;
    }

    return false;
}


void advcpp::Mutex::Destroy()
{
    if(m_isAvailableMutex.Check())
    {
        if(m_isLocked.Check())
        {
            pthread_mutex_unlock(&m_mutex);
        }

        assert(pthread_mutex_destroy(&m_mutex) == 0);
        m_isAvailableMutex.False();
    }
}


pthread_mutex_t& advcpp::Mutex::GetInnerMutex()
{
    return m_mutex;
}