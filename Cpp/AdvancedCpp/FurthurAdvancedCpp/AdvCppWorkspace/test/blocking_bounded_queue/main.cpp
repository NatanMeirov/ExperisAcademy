#include "mu_test.h"
#include "blocking_bounded_queue.hpp"


using namespace advcpp;


BEGIN_TEST(raw_queue_exceptions_check)
    constexpr size_t N = 100;
    Queue<int> rq(N);

    try
    {
        rq.Dequeue(); // Should throw (empty)
        ASSERT_FAIL("Exception should had thrown by now");
    }
    catch(...)
    {
    }

    for(size_t i = 0; i < N; ++i)
    {
        rq.Enqueue(i);
    }

    try
    {
        rq.Enqueue(0); // Should throw (full)
        ASSERT_FAIL("Exception should had thrown by now");
    }
    catch(...)
    {
    }

    ASSERT_PASS();
END_TEST


BEGIN_TEST(queue_capacity_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int> numbers(N);
    ASSERT_EQUAL(numbers.Capacity(), N);

    numbers.Enqueue(N);
    ASSERT_EQUAL(numbers.Capacity(), N);
END_TEST


BEGIN_TEST(queue_size_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int> numbers(N);
    for(size_t i = 0; i < N; ++i)
    {
        numbers.Enqueue(i);
    }
    ASSERT_EQUAL(numbers.Size(), N);

    for(size_t i = 0; i < N; ++i)
    {
        size_t popped = numbers.Dequeue();
        ASSERT_EQUAL(i, popped);
    }

    ASSERT_EQUAL(numbers.Size(), 0);
END_TEST


BEGIN_TEST(queue_is_full_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int> numbers(N);
    for(size_t i = 0; i < N; ++i)
    {
        ASSERT_THAT(!numbers.IsFull());
        numbers.Enqueue(i);
    }

    ASSERT_THAT(numbers.IsFull());

    numbers.Dequeue();

    ASSERT_THAT(!numbers.IsFull());
END_TEST


BEGIN_TEST(queue_is_empty_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int> numbers(N);
    ASSERT_THAT(numbers.IsEmpty());
    numbers.Enqueue(0);
    ASSERT_THAT(!numbers.IsEmpty());
    numbers.Dequeue();
    ASSERT_THAT(numbers.IsEmpty());

END_TEST


TEST_SUITE(BlockingBoundedQueueTest)

    TEST(raw_queue_exceptions_check)
    TEST(queue_capacity_check)
    TEST(queue_size_check)
    TEST(queue_is_full_check)
    TEST(queue_is_empty_check)

END_SUITE