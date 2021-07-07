#include "mu_test.h"
#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include <unistd.h> // sleep
#include "thread.hpp"
#include "thread_destruction_policies.hpp"
#include "icallable.hpp"
#include "counter.hpp"
#include "counter_increment_task.hpp"
#include "counter_decrement_task.hpp"


BEGIN_TEST(double_inc_threads_test_till_100)
    using advcpp::Thread;
    using advcpp::AssertionPolicy;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> firstIncThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> secondIncThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());

    firstIncThread.Join();
    secondIncThread.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(sync_massive_inc_threads_test_till_10000000_each)
    using advcpp::Thread;
    using advcpp::AssertionPolicy;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;

    constexpr size_t THREADS_NUM = 10;
    constexpr size_t N = 10000000;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread1(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread2(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread3(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread4(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread5(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread6(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread7(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread8(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread9(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> incThread10(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());

    incThread1.Join();
    incThread2.Join();
    incThread3.Join();
    incThread4.Join();
    incThread5.Join();
    incThread6.Join();
    incThread7.Join();
    incThread8.Join();
    incThread9.Join();
    incThread10.Join();


    TRACE(counter->Count());
    ASSERT_EQUAL(counter->Count(), N * THREADS_NUM); // sync check
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_10)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 10;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(counter, N)), AssertionPolicy());

    incThread.Join();
    decThread.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_10000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 10000;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(counter, N)), AssertionPolicy());

    incThread.Join();
    decThread.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_100000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 100000;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(counter, N)), AssertionPolicy());

    incThread.Join();
    decThread.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(inc_and_dec_threads_test_till_1000000)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;
    using advcpp::CounterDecrementTask;

    constexpr size_t N = 1000000;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());
    Thread<AssertionPolicy> decThread(std::shared_ptr<CounterDecrementTask>(new CounterDecrementTask(counter, N)), AssertionPolicy());

    incThread.Join();
    decThread.Join();

    TRACE(counter->Count());
    ASSERT_PASS();
END_TEST


BEGIN_TEST(double_join_and_exception)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100000;
    std::shared_ptr<Counter> counter(new Counter());
    Thread<AssertionPolicy> incThread(std::shared_ptr<CounterIncrementTask>(new CounterIncrementTask(counter, N)), AssertionPolicy());

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


BEGIN_TEST(race_condition_on_resources_check)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));

    {
        Thread<AssertionPolicy> largeAmountIncThread(task, AssertionPolicy());
        largeAmountIncThread.Detach();
    } // The Thread obj is destructed - its resources are cleaned

    ASSERT_THAT(task.use_count() == 2); // 1: the task ref (this test), 2: the routine (Task) ref (N is big enough to keep the routine "alive")
    TRACE(counter->Count());
    sleep(1); // To force the task to work
    TRACE(counter->Count());
END_TEST


BEGIN_TEST(assertion_policy_check)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 10000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));

    {
        Thread<AssertionPolicy> largeAmountIncThread(task, AssertionPolicy());
    } // The Thread obj is destructed - assertion policy should be triggered

    ASSERT_PASS(); // SHOULD NOT BE CALLED HERE
END_TEST


BEGIN_TEST(join_policy_check)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::JoinPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 10000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));

    {
        Thread<JoinPolicy> largeAmountIncThread(task, JoinPolicy());
    } // The Thread obj is destructed - join policy should be triggered

    ASSERT_EQUAL(counter->Count(), N); // After the join it should be N
END_TEST


BEGIN_TEST(detach_policy_check)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::DetachPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 10000000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));

    {
        Thread<DetachPolicy> largeAmountIncThread(task, DetachPolicy());
    } // The Thread obj is destructed - detach policy should be triggered

    size_t firstCountSave = counter->Count();
    sleep(2);
    size_t secondCountSave = counter->Count();

    ASSERT_NOT_EQUAL(firstCountSave, secondCountSave); // Detached thread should continue in the background => still counting
END_TEST


BEGIN_TEST(cancel_policy_check)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::CancelPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 1000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));

    {
        Thread<CancelPolicy> largeAmountIncThread(task, CancelPolicy(true));
    } // The Thread obj is destructed - cancel policy should be triggered

    size_t firstCountSave = counter->Count();
    TRACE(firstCountSave);

    sleep(2);

    size_t secondCountSave = counter->Count(); // should be same as the one before
    TRACE(secondCountSave);

    ASSERT_EQUAL(firstCountSave, secondCountSave); // Canceled thread should NOT continue in the background => counter is not changing
END_TEST


BEGIN_TEST(check_thread_has_finished_true)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 1;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));
    Thread<AssertionPolicy> fewIncThread(task, AssertionPolicy());
    fewIncThread.Join();

    sleep(1); // To ensure that the task has done by now

    ASSERT_THAT(fewIncThread.HasDone());
END_TEST


BEGIN_TEST(check_thread_has_finished_false)
    using advcpp::Thread;
    using advcpp::Counter;
    using advcpp::AssertionPolicy;
    using advcpp::CounterIncrementTask;

    constexpr size_t N = 100000000000;
    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> task(new CounterIncrementTask(counter, N));
    Thread<AssertionPolicy> largeAmountIncThread(task, AssertionPolicy());
    largeAmountIncThread.Detach(); // task has to count till 100000000 - pretty big task

    ASSERT_THAT(!largeAmountIncThread.HasDone());
END_TEST


BEGIN_TEST(atomic_value_operators_test)
    using namespace advcpp;
    constexpr size_t N = 100;
    constexpr size_t VAL = 100;

    AtomicValue<int> av(VAL);

    for(size_t i = 0; i < N; ++i)
    {
        // inc a
        auto t = av.Get();
        ++t;
        av.Set(t);

        --av;
    }

    ASSERT_EQUAL(av.Get(), VAL);
END_TEST


BEGIN_SUITE(ThreadsTest)
    TEST(double_inc_threads_test_till_100)
    TEST(sync_massive_inc_threads_test_till_10000000_each)
    TEST(inc_and_dec_threads_test_till_10)
    TEST(inc_and_dec_threads_test_till_10000)
    TEST(inc_and_dec_threads_test_till_100000)
    TEST(inc_and_dec_threads_test_till_1000000)
    TEST(double_join_and_exception)
    TEST(race_condition_on_resources_check)
    IGNORE_TEST(assertion_policy_check)
    TEST(join_policy_check)
    TEST(detach_policy_check)
    TEST(cancel_policy_check)
    TEST(check_thread_has_finished_true)
    TEST(check_thread_has_finished_false)
    TEST(atomic_value_operators_test)
END_SUITE
