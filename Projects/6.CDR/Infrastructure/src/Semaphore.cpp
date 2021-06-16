#include "../inc/Semaphore.hpp"
#include <semaphore.h> // Semaphore functions
#include <stdexcept> // std::runtime_error


nm::Semaphore::Semaphore(int a_sharedOption, unsigned int a_initialValue)
: m_semaphore()
, m_isAvailableSemaphore(true) {
    int statusCode = sem_init(&this->m_semaphore, a_sharedOption, a_initialValue);
    if(statusCode != 0) {
        this->m_isAvailableSemaphore = false;
        throw std::runtime_error("Failed while trying to initialize the Semaphore");
    }
}


nm::Semaphore::~Semaphore() {
    this->Destroy();
}


unsigned int nm::Semaphore::GetCurrentValue() {
    if(this->m_isAvailableSemaphore) {
        int currentValue;
        sem_getvalue(&this->m_semaphore, &currentValue);

        return currentValue;
    }

    return 0;
}


void nm::Semaphore::Up() {
    if(this->m_isAvailableSemaphore) {
        int statusCode = sem_post(&this->m_semaphore);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to post");
        }
    }
}


void nm::Semaphore::Down() {
    if(this->m_isAvailableSemaphore) {
        int statusCode = sem_wait(&this->m_semaphore);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to wait");
        }
    }
}


bool nm::Semaphore::TryDown() {
    if(this->m_isAvailableSemaphore) {
        return (sem_trywait(&this->m_semaphore) == 0) ? true : false;
    }

    return false;
}


void nm::Semaphore::Destroy() {
    if(this->m_isAvailableSemaphore) {
        sem_destroy(&this->m_semaphore);
        this->m_isAvailableSemaphore = false;
    }
}