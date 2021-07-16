#include "mu_test.h"
#include <unistd.h> // sleep
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "icallable.hpp"
#include "thread_pool.hpp"
#include "counter.hpp"
#include "counter_increment_task.hpp"
#include "thread_pool_destruction_policies.hpp"


BEGIN_TEST(thread_pool_submit_and_add_check)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;

    constexpr size_t N = 10000;
    constexpr size_t QUEUE_SIZE = 100;
    constexpr size_t WORKS_COUNT = 2;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<ShutdownPolicy<>> pool(ShutdownPolicy<>(), QUEUE_SIZE, 0);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    ASSERT_EQUAL(pool.PendingWorksCount(), WORKS_COUNT);

    pool.AddWorkers(1);
    ASSERT_EQUAL(pool.WorkersCount(), 1);

    sleep(1);
    ASSERT_NOT_EQUAL(pool.PendingWorksCount(), WORKS_COUNT);

    pool.AddWorkers(1);
    ASSERT_EQUAL(pool.WorkersCount(), 2);
END_TEST


BEGIN_TEST(thread_pool_submit_shutdown_check)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::AssertingPolicy;

    constexpr size_t N = 1000000;
    constexpr size_t WORKERS_N = 2;
    constexpr size_t QUEUE_SIZE = 5;
    constexpr size_t WORKS_COUNT = 15;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<AssertingPolicy<>> pool(AssertingPolicy<>(), QUEUE_SIZE, WORKERS_N);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    pool.Shutdown();

    TRACE(counter->Count());
    ASSERT_EQUAL(counter->Count(), WORKS_COUNT * N);
END_TEST


BEGIN_TEST(thread_pool_shutdown_immidiate_on_working_check)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;
    using advcpp::AssertingPolicy;

    constexpr size_t N = 1000000;
    constexpr size_t WORKERS_N = 2;
    constexpr size_t QUEUE_SIZE = 25;
    constexpr size_t WORKS_COUNT = 20;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<AssertingPolicy<>> pool(AssertingPolicy<>(), QUEUE_SIZE, WORKERS_N);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    pool.ShutdownImmediate();

    TRACE(counter->Count());
    ASSERT_NOT_EQUAL(counter->Count(), WORKS_COUNT * N);
END_TEST


BEGIN_TEST(thread_pool_shutdown_immidiate_waiting_on_dequeue_check)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;
    using advcpp::AssertingPolicy;

    constexpr size_t N = 1000000;
    constexpr size_t WORKERS_N = 2;
    constexpr size_t QUEUE_SIZE = 5;
    constexpr size_t WORKS_COUNT = 0;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<AssertingPolicy<>> pool(AssertingPolicy<>(), QUEUE_SIZE, WORKERS_N);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    pool.ShutdownImmediate();
    ASSERT_PASS();
END_TEST


BEGIN_TEST(thread_pool_shutdown_immidiate_while_waiting_on_dequeue_check)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;
    using advcpp::AssertingPolicy;

    constexpr size_t N = 1000000;
    constexpr size_t WORKERS_N = 4;
    constexpr size_t QUEUE_SIZE = 10;
    constexpr size_t WORKS_COUNT = 9;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<AssertingPolicy<>> pool(AssertingPolicy<>(), QUEUE_SIZE, WORKERS_N);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    pool.ShutdownImmediate();

    TRACE(counter->Count());
    ASSERT_NOT_EQUAL(counter->Count(), WORKS_COUNT * N);
END_TEST


BEGIN_TEST(thread_pool_remove_worker_empty_queue)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;

    constexpr size_t N = 100000;
    constexpr size_t WORKERS_N = 4;
    constexpr size_t QUEUE_SIZE = 10;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<ShutdownPolicy<>> pool(ShutdownPolicy<>(), QUEUE_SIZE, WORKERS_N);

    TRACE(pool.WorkersCount());
    pool.RemoveWorkers(2);
    TRACE(pool.WorkersCount());
    ASSERT_EQUAL(pool.WorkersCount(), WORKERS_N - 2);
    pool.Shutdown();
END_TEST



BEGIN_TEST(thread_pool_remove_while_executing_work)
    using advcpp::ThreadPool;
    using advcpp::Counter;
    using advcpp::CounterIncrementTask;
    using advcpp::ClearPolicy;
    using advcpp::ICallable;
    using advcpp::BlockingBoundedQueue;
    using advcpp::ShutdownPolicy;

    constexpr size_t N = 100000;
    constexpr size_t WORKERS_N = 2;
    constexpr size_t QUEUE_SIZE = 10;
    constexpr size_t WORKS_COUNT = 4;

    std::shared_ptr<Counter> counter(new Counter());
    std::shared_ptr<CounterIncrementTask> work(new CounterIncrementTask(counter, N));

    ThreadPool<ShutdownPolicy<>> pool(ShutdownPolicy<>(), QUEUE_SIZE, WORKERS_N);

    for(size_t i = 0; i < WORKS_COUNT; ++i)
    {
        pool.SubmitWork(work);
    }

    pool.RemoveWorkers(1);
    TRACE(pool.WorkersCount());
    ASSERT_EQUAL(pool.WorkersCount(), WORKERS_N - 1);
END_TEST



BEGIN_SUITE(ThreadPoolTests)

    // TEST(thread_pool_submit_and_add_check)
    // TEST(thread_pool_submit_shutdown_check)
    // TEST(thread_pool_shutdown_immidiate_on_working_check)
    // TEST(thread_pool_shutdown_immidiate_waiting_on_dequeue_check)
    // TEST(thread_pool_shutdown_immidiate_while_waiting_on_dequeue_check)
    TEST(thread_pool_remove_worker_empty_queue)
    // TEST(thread_pool_remove_while_executing_work)


END_SUITE
