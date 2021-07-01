#include "mu_test.h"
#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include "thread.hpp"
#include "icallable.hpp"
#include "counter.hpp"
#include "counter_increment_task.hpp"
#include "counter_decrement_task.hpp"


BEGIN_TEST(double_inc_threads_test_till_100)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100;
    Counter globalCounter;
    Thread firstIncThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);
    Thread secondIncThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);

    firstIncThread.Join();
    secondIncThread.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_10)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 10;
    Counter globalCounter;
    Thread incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);
    Thread decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(globalCounter, N)), Thread::JOIN);

    incThread.Join();
    decThread.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_10000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 10000;
    Counter globalCounter;
    Thread incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);
    Thread decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(globalCounter, N)), Thread::JOIN);

    incThread.Join();
    decThread.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_100000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 100000;
    Counter globalCounter;
    Thread incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);
    Thread decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(globalCounter, N)), Thread::JOIN);

    incThread.Join();
    decThread.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_1000000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 1000000;
    Counter globalCounter;
    Thread incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);
    Thread decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(globalCounter, N)), Thread::JOIN);

    incThread.Join();
    decThread.Join();

    TRACE(globalCounter.Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(double_join_and_exception)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100000;
    Counter globalCounter;
    Thread incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(globalCounter, N)), Thread::JOIN);

    try
    {
        incThread.Join();
        incThread.Join(); // Should throw
        ASSERT_FAIL("An exception should have thrown by now...");
    }
    catch (...)
    {
        ASSERT_PASS();
    }
END_TEST


BEGIN_SUITE(ThreadsTest)
    TEST(double_inc_threads_test_till_100)
    TEST(inc_and_dec_threads_test_till_10)
    TEST(inc_and_dec_threads_test_till_10000)
    TEST(inc_and_dec_threads_test_till_100000)
    TEST(inc_and_dec_threads_test_till_1000000)
    TEST(double_join_and_exception)
END_SUITE
