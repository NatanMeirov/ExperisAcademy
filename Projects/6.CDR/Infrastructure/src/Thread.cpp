#include "../inc/Thread.hpp"
#include <pthread.h> // pthread_create, pthread_join, pthread_exit
#include <stdexcept> // std::runtime_error


nm::Thread::Thread(TaskFunction a_taskToExecute, void* a_context)
: m_threadID()
, m_taskToExecute(a_taskToExecute)
, m_context(a_context)
, m_isAvailableThread(true) {
    int statusCode = pthread_create(&this->m_threadID, NULL, this->m_taskToExecute, this->m_context);
    if(statusCode != 0) {
        this->m_isAvailableThread = false;
        throw std::runtime_error("Failed while trying to initialize the Thread");
    }
}


nm::Thread::Thread(TaskFunction a_taskToExecute)
: Thread(a_taskToExecute, nullptr) {
}


nm::Thread::~Thread() {
    this->m_isAvailableThread = false;
}


void* nm::Thread::Join() {
    if(this->m_isAvailableThread) {
        void* taskReturnedValue = nullptr;
        int statusCode = pthread_join(this->m_threadID, &taskReturnedValue);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to join");
        }

        this->m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing

        return taskReturnedValue;
    }

    return nullptr;
}


void nm::Thread::Detach() {
    if(this->m_isAvailableThread) {
        int statusCode = pthread_detach(this->m_threadID);
        if(statusCode != 0) {
            throw std::runtime_error("Failed while trying to join");
        }

        this->m_isAvailableThread = false; // Not critical if an exception is thrown and this line is not executing
    }
}