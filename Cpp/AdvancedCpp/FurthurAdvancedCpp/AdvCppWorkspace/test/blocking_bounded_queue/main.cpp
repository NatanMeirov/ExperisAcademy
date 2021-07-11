#include "mu_test.h"
#include <memory> // std::shared_ptr, std::make_shared
#include <algorithm> // std::is_sorted
#include "blocking_bounded_queue.hpp"
#include "thread.hpp"
#include "producer_task.hpp"
#include "consumer_task.hpp"
#include "thread_destruction_policies.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"


using namespace advcpp;


BEGIN_TEST(queue_assert_policy_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int, AssertPolicy<int>> numbers(N, AssertPolicy<int>());
    numbers.Enqueue(1); // Assert should be called on not empty queue
END_TEST


BEGIN_TEST(queue_no_operation_policy_check)
    constexpr size_t N = 100;

    {
        BlockingBoundedQueue<int, NoOperationPolicy<int>> numbers(N, NoOperationPolicy<int>());
        numbers.Enqueue(1);
        numbers.Enqueue(2);
        numbers.Enqueue(3);
    }
    ASSERT_PASS();
END_TEST


BEGIN_TEST(queue_clear_policy_check)
    constexpr size_t N = 100;

    {
        BlockingBoundedQueue<int, ClearPolicy<int>> numbers(N, ClearPolicy<int>());
        numbers.Enqueue(1);
        numbers.Enqueue(2);
        numbers.Enqueue(3);
    }
    ASSERT_PASS();

END_TEST


BEGIN_TEST(queue_save_policy_check)
    constexpr size_t N = 100;

    std::shared_ptr<std::vector<int>> vectorPtr(new std::vector<int>());
    {
        BlockingBoundedQueue<int, SavePolicy<int,std::vector<int>>> numbers(N, SavePolicy<int,std::vector<int>>(vectorPtr));
        numbers.Enqueue(1);
        numbers.Enqueue(2);
        numbers.Enqueue(3);
    }
    ASSERT_THAT((*vectorPtr)[0] == 1 && (*vectorPtr)[1] == 2 && (*vectorPtr)[2] == 3);
END_TEST


BEGIN_TEST(queue_callback_policy_check)
    constexpr size_t N = 100;

    auto printInt = [](int a_number){ std::cout << a_number << " "; };
    {
        BlockingBoundedQueue<int, CallbackPolicy<int,decltype(printInt)>> numbers(N, CallbackPolicy<int,decltype(printInt)>(printInt));
        numbers.Enqueue(1);
        numbers.Enqueue(2);
        numbers.Enqueue(3);
    }
    std::cout << "\nExpected: 1 2 3" << std::endl;
    ASSERT_PASS();
END_TEST



BEGIN_TEST(queue_capacity_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int, ClearPolicy<int>> numbers(N, ClearPolicy<int>());
    ASSERT_EQUAL(numbers.Capacity(), N);

    numbers.Enqueue(N);
    ASSERT_EQUAL(numbers.Capacity(), N);
END_TEST


BEGIN_TEST(queue_size_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int, ClearPolicy<int>> numbers(N, ClearPolicy<int>());
    for(size_t i = 0; i < N; ++i)
    {
        numbers.Enqueue(i);
    }
    ASSERT_EQUAL(numbers.Size(), N);

    for(int i = 0; i < int(N); ++i)
    {
        int num;
        numbers.Dequeue(num);
        ASSERT_EQUAL(i, num);
    }

    ASSERT_EQUAL(numbers.Size(), 0);
END_TEST


BEGIN_TEST(queue_is_full_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int, ClearPolicy<int>> numbers(N, ClearPolicy<int>());
    for(size_t i = 0; i < N; ++i)
    {
        ASSERT_THAT(!numbers.IsFull());
        numbers.Enqueue(i);
    }

    ASSERT_THAT(numbers.IsFull());

    int num = N;
    numbers.Dequeue(num);

    ASSERT_EQUAL(num, 0); // Dequeue - the first number in the queue (from head of queue is 0)
    ASSERT_THAT(!numbers.IsFull());
END_TEST


BEGIN_TEST(queue_is_empty_check)
    constexpr size_t N = 100;

    BlockingBoundedQueue<int, AssertPolicy<int>> numbers(N, AssertPolicy<int>());
    ASSERT_THAT(numbers.IsEmpty());
    numbers.Enqueue(N);
    ASSERT_THAT(!numbers.IsEmpty());
    int num = 0;
    numbers.Dequeue(num);
    ASSERT_THAT(numbers.IsEmpty());
    ASSERT_EQUAL(num, N);
END_TEST


BEGIN_TEST(queue_one_consumer_one_producer)
    constexpr size_t N = 10000000;
    std::shared_ptr<BlockingBoundedQueue<int, AssertPolicy<int>>> numbers = std::make_shared<BlockingBoundedQueue<int, AssertPolicy<int>>>((N/1000), AssertPolicy<int>());
    std::shared_ptr<std::vector<int>> resultVector(new std::vector<int>());
    resultVector->reserve(N);
    std::mutex lock;

    std::shared_ptr<ProducerTask> producerTask = std::make_shared<ProducerTask>(numbers, N);
    std::shared_ptr<ConsumerTask> consumerTask = std::make_shared<ConsumerTask>(numbers, N, resultVector, lock);

    Thread<AssertionPolicy> producer(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer(consumerTask, AssertionPolicy());

    producer.Join();
    consumer.Join();

    ASSERT_EQUAL(resultVector->size(), N);
    ASSERT_THAT(numbers->IsEmpty());
    ASSERT_THAT(std::is_sorted(resultVector->begin(), resultVector->end()));
END_TEST


BEGIN_TEST(queue_one_consumer_two_producers)
    constexpr size_t N = 10000000;
    std::shared_ptr<BlockingBoundedQueue<int, AssertPolicy<int>>> numbers = std::make_shared<BlockingBoundedQueue<int, AssertPolicy<int>>>((N/1000), AssertPolicy<int>());
    std::shared_ptr<std::vector<int>> resultVector(new std::vector<int>());
    resultVector->reserve(N);
    std::mutex lock;

    std::shared_ptr<ProducerTask> producerTask = std::make_shared<ProducerTask>(numbers, (N/2));
    std::shared_ptr<ConsumerTask> consumerTask = std::make_shared<ConsumerTask>(numbers, N, resultVector, lock);

    Thread<AssertionPolicy> producer1(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> producer2(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer(consumerTask, AssertionPolicy());

    producer1.Join();
    producer2.Join();
    consumer.Join();

    ASSERT_EQUAL(resultVector->size(), N);
    ASSERT_THAT(numbers->IsEmpty());
END_TEST


BEGIN_TEST(queue_two_consumers_one_producer)
    constexpr size_t N = 10000000;
    std::shared_ptr<BlockingBoundedQueue<int, AssertPolicy<int>>> numbers = std::make_shared<BlockingBoundedQueue<int, AssertPolicy<int>>>((N/1000), AssertPolicy<int>());
    std::shared_ptr<std::vector<int>> resultVector(new std::vector<int>());
    resultVector->reserve(N);
    std::mutex lock;

    std::shared_ptr<ProducerTask> producerTask = std::make_shared<ProducerTask>(numbers, N);
    std::shared_ptr<ConsumerTask> consumerTask = std::make_shared<ConsumerTask>(numbers, (N/2), resultVector, lock);

    Thread<AssertionPolicy> producer(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer1(consumerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer2(consumerTask, AssertionPolicy());

    producer.Join();
    consumer1.Join();
    consumer2.Join();

    ASSERT_EQUAL(resultVector->size(), N);
    ASSERT_THAT(numbers->IsEmpty());
END_TEST


BEGIN_TEST(queue_two_consumers_two_producers)
    constexpr size_t N = 10000000;
    std::shared_ptr<BlockingBoundedQueue<int, AssertPolicy<int>>> numbers = std::make_shared<BlockingBoundedQueue<int, AssertPolicy<int>>>((N/1000), AssertPolicy<int>());
    std::shared_ptr<std::vector<int>> resultVector(new std::vector<int>());
    resultVector->reserve(N);
    std::mutex lock;

    std::shared_ptr<ProducerTask> producerTask = std::make_shared<ProducerTask>(numbers, (N/2));
    std::shared_ptr<ConsumerTask> consumerTask = std::make_shared<ConsumerTask>(numbers, (N/2), resultVector, lock);

    Thread<AssertionPolicy> producer1(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> producer2(producerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer1(consumerTask, AssertionPolicy());
    Thread<AssertionPolicy> consumer2(consumerTask, AssertionPolicy());

    producer1.Join();
    producer2.Join();
    consumer1.Join();
    consumer2.Join();

    ASSERT_EQUAL(resultVector->size(), N);
    ASSERT_THAT(numbers->IsEmpty());
END_TEST


TEST_SUITE(BlockingBoundedQueueTest)

    IGNORE_TEST(queue_assert_policy_check)
    TEST(queue_no_operation_policy_check)
    TEST(queue_clear_policy_check)
    TEST(queue_save_policy_check)
    TEST(queue_callback_policy_check)
    TEST(queue_capacity_check)
    TEST(queue_size_check)
    TEST(queue_is_full_check)
    TEST(queue_is_empty_check)
    TEST(queue_one_consumer_one_producer)
    TEST(queue_one_consumer_two_producers)
    TEST(queue_two_consumers_one_producer)
    TEST(queue_two_consumers_two_producers)

END_SUITE
