#include "mu_test.h"
#include <cstddef> // size_t
#include <unistd.h> // sleep
#include <memory> // std::shared_ptr
#include "thread_group.hpp"
#include "thread.hpp"
#include "thread_destruction_policies.hpp"
#include "icallable.hpp"
#include "counter.hpp"
#include "counter_increment_task.hpp"
#include "counter_decrement_task.hpp"


BEGIN_TEST(inc_threads_group_test_till_100_X_4)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::AssertionPolicy;

    constexpr size_t TN = 4;
    constexpr size_t N = 100;

    std::shared_ptr<Counter> counter(new Counter());
    ThreadGroup<AssertionPolicy> threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), TN, AssertionPolicy());

    threadsGroup.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_threads_group_size_clear_and_add_check)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::JoinPolicy;

    constexpr size_t TN = 4;
    constexpr size_t ADDED_T = 2;
    constexpr size_t N = 100;

    std::shared_ptr<Counter> counter(new Counter());
    ThreadGroup<JoinPolicy> threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), TN, JoinPolicy());

    ASSERT_EQUAL(threadsGroup.Size(), TN);
    sleep(1); // should let them finish - and be ready for clear
    threadsGroup.Add(ADDED_T);
    ASSERT_EQUAL(threadsGroup.Size(), ADDED_T);
END_TEST


BEGIN_TEST(big_inc_threads_group_size_remove_without_clear_check)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::DetachPolicy;

    constexpr size_t TN = 4;
    constexpr size_t REMOVE_T = 2;
    constexpr size_t REMAINING_T = 2;
    constexpr size_t N = 1000000000;

    std::shared_ptr<Counter> counter(new Counter());
    ThreadGroup<DetachPolicy> threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), TN, DetachPolicy());

    ASSERT_EQUAL(threadsGroup.Size(), TN);
    threadsGroup.Remove(REMOVE_T);
    ASSERT_EQUAL(threadsGroup.Size(), REMAINING_T);
    threadsGroup.Remove(REMOVE_T);
    ASSERT_EQUAL(threadsGroup.Size(), 0);
END_TEST


BEGIN_SUITE(ThreadsTest)
    TEST(inc_threads_group_test_till_100_X_4)
    TEST(inc_threads_group_size_clear_and_add_check)
    TEST(big_inc_threads_group_size_remove_without_clear_check)
END_SUITE
