#ifndef NM_THREAD_GROUP_HPP
#define NM_THREAD_GROUP_HPP


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "thread.hpp"


namespace advcpp
{

class ThreadGroup
{
public:
    enum GroupDestructionAction { JOIN, DETACH, CANCEL };

    ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, GroupDestructionAction a_destructionActionIndicator);
    ThreadGroup(const ThreadGroup& a_other) = delete;
    ThreadGroup& operator=(const ThreadGroup& a_other) = delete;
    ~ThreadGroup(); // Each Thread handles its destruction, specified by DestructionAction option

    void Join();
    void Detach();
    void Cancel();

private:
    static constexpr Thread::DestructionAction DESTRUCT_THREAD_OPT[] = {Thread::JOIN, Thread::DETACH, Thread::CANCEL};

private:
    std::vector<std::shared_ptr<Thread>> m_threadsGroup;
    bool m_isAvailableThreadGroup;
};

} // advcpp


#endif // NM_THREAD_GROUP_HPP