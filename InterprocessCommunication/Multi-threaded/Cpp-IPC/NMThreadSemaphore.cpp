#include "NMThreadSemaphore.hpp"
#include <pthread.h> // mutex and conditional variable functions


nm::NMThreadSemaphore::NMThreadSemaphore(unsigned int a_count)
: m_mutexLock(PTHREAD_MUTEX_INITIALIZER)
, m_conditionalVariable(PTHREAD_COND_INITIALIZER)
, m_count(a_count) {
}


nm::NMThreadSemaphore::~NMThreadSemaphore() {
    pthread_mutex_destroy(&this->m_mutexLock);
    pthread_cond_destroy(&this->m_conditionalVariable);
}


unsigned int nm::NMThreadSemaphore::GetCurrentCount() const {
    return this->m_count;
}


void nm::NMThreadSemaphore::Down() {
    pthread_mutex_lock(&this->m_mutexLock);
    while(this->m_count == 0) {
        pthread_cond_wait(&this->m_conditionalVariable, &this->m_mutexLock);
    }
    --this->m_count;
    pthread_mutex_unlock(&this->m_mutexLock);
}


void nm::NMThreadSemaphore::Up() {
    pthread_mutex_lock(&this->m_mutexLock);
    ++this->m_count;
    pthread_cond_signal(&this->m_conditionalVariable);
    pthread_mutex_unlock(&this->m_mutexLock);
}