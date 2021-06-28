#include "Mutex.hpp"
#include <pthread.h> // Mutex functions and MACROS
#include <stdexcept> // std::runtime_error


nm::Mutex::Mutex()
: m_mutex(PTHREAD_MUTEX_INITIALIZER)
, m_isAvailableMutex(true)
, m_isLocked(false) {
}


nm::Mutex::~Mutex() {
    if(this->m_isAvailableMutex) {
        this->Destroy();
    }
}


void nm::Mutex::Lock() {
    if(this->m_isAvailableMutex) {
        if(this->m_isLocked) {
            throw std::runtime_error("Mutex is already locked");
        }

        int statusCode = pthread_mutex_lock(&this->m_mutex);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to lock");
        }

        this->m_isLocked = true;
    }
}


void nm::Mutex::Unlock() {
    if(this->m_isAvailableMutex) {
        if(!this->m_isLocked) {
            throw std::runtime_error("Mutex is already unlocked");
        }

        int statusCode = pthread_mutex_unlock(&this->m_mutex);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to unlock");
        }

        this->m_isLocked = false;
    }
}


bool nm::Mutex::TryLock() {
    if(this->m_isAvailableMutex) {
        return (pthread_mutex_trylock(&this->m_mutex) == 0) ? true : false;
    }

    return false;
}


void nm::Mutex::Destroy() {
    if(this->m_isAvailableMutex) {
        if(this->m_isLocked) {
            pthread_mutex_unlock(&this->m_mutex);
        }
        pthread_mutex_destroy(&this->m_mutex);
        this->m_isAvailableMutex = false;
    }
}


pthread_mutex_t& nm::Mutex::GetInnerMutex() {
    return this->m_mutex;
}