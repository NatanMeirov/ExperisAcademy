#include "mu_test.h"
#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include "thread_group.hpp"
#include "thread.hpp"
#include "icallable.hpp"
#include "counter.hpp"
#include "counter_increment_task.hpp"
#include "counter_decrement_task.hpp"


BEGIN_TEST(inc_threads_group_test_till_100_X_4)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t TN = 4;
    constexpr size_t N = 100;
    Counter globalCounter;

    ThreadGroup threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), TN, ThreadGroup::JOIN);

    threadsGroup.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_threads_group_test_till_100000_X_4)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t TN = 4;
    constexpr size_t N = 100000;
    Counter globalCounter;

    ThreadGroup threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), TN, ThreadGroup::JOIN);

    threadsGroup.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(double_join_group_and_exception)
    using advcpp::ThreadGroup;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t TN = 4;
    constexpr size_t N = 10;
    Counter globalCounter;

    ThreadGroup threadsGroup(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), TN, ThreadGroup::JOIN);

    try
    {
        threadsGroup.Join();
        threadsGroup.Join(); // Should throw
        ASSERT_FAIL("An exception should have thrown by now...");
    }
    catch (...)
    {
        ASSERT_PASS();
    }
END_TEST


BEGIN_SUITE(ThreadsTest)
    TEST(inc_threads_group_test_till_100_X_4)
    TEST(inc_threads_group_test_till_100000_X_4)
    TEST(double_join_group_and_exception)
END_SUITE
