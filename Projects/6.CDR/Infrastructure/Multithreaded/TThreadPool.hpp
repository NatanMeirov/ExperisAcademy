/* A compile time (template) ThreadPool, using Functos as Tasks */

#ifndef __NM_TTHREADPOOL_HPP__
#define __NM_TTHREADPOOL_HPP__


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "Thread.hpp"
#include "SafeQueue_Inline.hpp"


namespace nm {

namespace compiletime {

// The TaskFunctor should contain the WHOLE DATA required to its execution - to be passed into the working threads (as their context), and the TaskFunctor should not get any paramether for its execution!
// TaskFunctor Concept: MUST be copy-constructable
template <typename TaskFunctor>
class ThreadPool {
public:
    ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue);

    void PushWork(const TaskFunctor& a_task);
    void StopWork(); // Stop TP from working - Would cancel all the running Threads

    struct ThreadPoolSharedData {
        ThreadPoolSharedData(const size_t a_workingQueueInitialSize) : m_tasksQueue(a_workingQueueInitialSize), m_isRequiredStopThreadPoolFromRunning(false) {}

        SafeQueue<TaskFunctor> m_tasksQueue;
        bool m_isRequiredStopThreadPoolFromRunning;
    };

private:
    std::vector<Thread> m_workers;
    std::shared_ptr<ThreadPoolSharedData> m_threadPoolSharedData;
};


// ThreadPool Inline:

template <typename TaskFunctor>
inline static void* ThreadPoolWorkingProcess(void* a_context) {
    typename ThreadPool<TaskFunctor>::ThreadPoolSharedData* globalThreadPoolData = static_cast<typename ThreadPool<TaskFunctor>::ThreadPoolSharedData*>(a_context);

    while(!globalThreadPoolData->m_isRequiredStopThreadPoolFromRunning || !globalThreadPoolData->m_tasksQueue.IsEmpty()) {
        TaskFunctor task = globalThreadPoolData->m_tasksQueue.Dequeue();
        task(); // Task Execution
    }

    return nullptr;
}


template <typename TaskFunctor>
ThreadPool<TaskFunctor>::ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue)
: m_workers()
, m_threadPoolSharedData(new ThreadPoolSharedData(a_sizeOfWorkingQueue)) {
    for(size_t i = 0; i < a_workingThreadsNumber; ++i) {
        this->m_workers.push_back(nm::Thread(&ThreadPoolWorkingProcess<TaskFunctor>, static_cast<void*>(this->m_threadPoolSharedData.get())));
    }
}


template <typename TaskFunctor>
void ThreadPool<TaskFunctor>::PushWork(const TaskFunctor& a_task) {
    if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
        this->m_threadPoolSharedData->m_tasksQueue.Enqueue(a_task);
    }
}


template <typename TaskFunctor>
void ThreadPool<TaskFunctor>::StopWork() {
    this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning = true;
    while(!this->m_threadPoolSharedData->m_tasksQueue.IsEmpty()); // A polling loop to check if the safe queue is empty

    for(size_t i = 0; i < this->m_workers.size(); ++i) { // A "cleaning" loop (to ensure that all the threads has finished and did not block)
        try {
            this->m_workers.at(i).Cancel();
        }
        catch(const std::exception& ex) {
            // No-Op - it is a valid situation, because the thread may already has finished its running
        }
    }
}

} // compiletime

} // nm


#endif // __NM_TTHREADPOOL_HPP__