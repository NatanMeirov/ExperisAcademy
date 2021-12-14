#ifndef NM_THREAD_GROUP_HPP
#define NM_THREAD_GROUP_HPP


#include <cstddef> // size_t
#include <vector>
#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "thread.hpp"


namespace advcpp
{

/* Warning: The given ICallable task will be SHARED in all the N threads - make sure that the given task is thread-safe! */
class ThreadGroup
{
public:
    enum GroupDestructionAction { JOIN, DETACH, CANCEL, ASSERTION };

    ThreadGroup(std::shared_ptr<ICallable> a_commonTask, const size_t a_threadsCount, GroupDestructionAction a_destructionActionIndicator = JOIN);
    ThreadGroup(const ThreadGroup& a_other) = delete;
    ThreadGroup& operator=(const ThreadGroup& a_other) = delete;
    ~ThreadGroup(); // Each Thread handles its destruction, specified by DestructionAction option

    void Join();
    void Detach();
    void Cancel(bool a_ensureCompleteCancelation = false);

private:
    static constexpr Thread::DestructionAction DESTRUCT_THREAD_OPT[] = {Thread::JOIN, Thread::DETACH, Thread::CANCEL, Thread::ASSERTION};

private:
    std::vector<std::shared_ptr<Thread>> m_threadsGroup;
};

} // advcpp


#endif // NM_THREAD_GROUP_HPP