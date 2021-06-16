#include "../inc/Mutex.hpp"
#include <pthread.h> // Mutex functions and MACROS
#include <stdexcept> // std::runtime_error


nm::Mutex::Mutex()
: m_mutex(PTHREAD_MUTEX_INITIALIZER)
, m_isAvailableMutex(true) {
}


nm::Mutex::~Mutex() {
    this->Destroy();
}


void nm::Mutex::Lock() {
    if(this->m_isAvailableMutex) {
        int statusCode = pthread_mutex_lock(&this->m_mutex);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to lock");
        }
    }
}


void nm::Mutex::Unlock() {
    if(this->m_isAvailableMutex) {
        int statusCode = pthread_mutex_unlock(&this->m_mutex);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to unlock");
        }
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
        pthread_mutex_unlock(&this->m_mutex);
        pthread_mutex_destroy(&this->m_mutex);
        this->m_isAvailableMutex = false;
    }
}


pthread_mutex_t& nm::Mutex::GetInnerMutex() {
    return this->m_mutex;
}