#include "ThreadPool.hpp"
#include <cstddef> // size_t
#include <vector>
// #include <memory> // std::shared_ptr
#include "Thread.hpp"
#include "SafeQueue_Inline.hpp"
#include "ICommand.hpp"


static void* ThreadPoolWorkingProcess(void* a_context) {
    nm::runtime::ThreadPool::ThreadPoolSharedData* globalThreadPoolData = static_cast<nm::runtime::ThreadPool::ThreadPoolSharedData*>(a_context);

    while(!globalThreadPoolData->m_isRequiredStopThreadPoolFromRunning || !globalThreadPoolData->m_tasksQueue.IsEmpty()) {
        // std::shared_ptr<nm::ICommand> task = globalThreadPoolData->m_tasksQueue.Dequeue();
        nm::ICommand* task = globalThreadPoolData->m_tasksQueue.Dequeue();
        task->Execute(); // Task Execution
    }

    return nullptr;
}


nm::runtime::ThreadPool::ThreadPool(const size_t a_workingThreadsNumber, const size_t a_sizeOfWorkingQueue)
: m_workers()
, m_threadPoolSharedData(new ThreadPoolSharedData(a_sizeOfWorkingQueue)) {
    for(size_t i = 0; i < a_workingThreadsNumber; ++i) {
        this->m_workers.push_back(std::move(nm::Thread(&ThreadPoolWorkingProcess, static_cast<void*>(this->m_threadPoolSharedData.get()))));
    }
}


nm::runtime::ThreadPool::~ThreadPool() {
    if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
        this->StopExecution();
    }
    // Else - already stopped
}


void nm::runtime::ThreadPool::PushTask(ICommand* a_task) {
    if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
        this->m_threadPoolSharedData->m_tasksQueue.Enqueue(a_task);
    }
}


// void nm::runtime::ThreadPool::PushTask(std::shared_ptr<ICommand> a_task) {
//     if(!this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning) {
//         this->m_threadPoolSharedData->m_tasksQueue.Enqueue(a_task);
//     }
// }


// void nm::runtime::ThreadPool::PushTask(ICommand* a_task) {
//     this->PushTask(std::shared_ptr<ICommand>(a_task));
// }


void nm::runtime::ThreadPool::StopExecution() {
    this->m_threadPoolSharedData->m_isRequiredStopThreadPoolFromRunning = true;
    while(!this->m_threadPoolSharedData->m_tasksQueue.IsEmpty()); // A polling loop to check if the safe queue is empty

    for(size_t i = 0; i < this->m_workers.size(); ++i) { // A "cleaning" loop (to ensure that all the threads has finished and did not block)
        try {
            this->m_workers.at(i).Cancel();
        }
        catch(...) {
            // No-Op - it is a valid situation, because the thread may already has finished its running
        }
    }
}