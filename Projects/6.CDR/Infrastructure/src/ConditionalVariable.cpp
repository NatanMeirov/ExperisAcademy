#include "../inc/ConditionalVariable.hpp"
#include <pthread.h> // mutex and conditional variable functions
#include <stdexcept> // std::runtime_error


nm::ConditionalVariable::ConditionalVariable()
: m_conditionalVariable(PTHREAD_COND_INITIALIZER)
, m_isAvailableCondVar(true){
}


nm::ConditionalVariable::~ConditionalVariable() {
    if(this->m_isAvailableCondVar) {
        pthread_cond_destroy(&this->m_conditionalVariable);
        this->m_isAvailableCondVar = false;
    }
}


void nm::ConditionalVariable::Wait(nm::Mutex& a_mutex) {
    if(this->m_isAvailableCondVar) {
        int statusCode = pthread_cond_wait(&this->m_conditionalVariable, &a_mutex.GetInnerMutex());
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to wait");
        }
    }
}


void nm::ConditionalVariable::Signal() {
    if(this->m_isAvailableCondVar) {
        pthread_cond_signal(&this->m_conditionalVariable); // No error can occur
    }
}