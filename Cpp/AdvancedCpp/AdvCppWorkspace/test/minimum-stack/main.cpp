#include "mu_test.h"
#include <vector>
#include <array>
#include <algorithm> // std::transform
#include "minimum_stack.hpp"
#include "ball.hpp"



// template <typename T, typename ...Types>
// T Create(Types... a_args)
// {
//     return T(a_args...);
// }


// Should be lambda...
// nm::advcpp::Ball CreateBlackBall(const size_t a_radius)
// {
//     return nm::advcpp::Ball(a_radius, nm::advcpp::Ball::BLACK);
// }


BEGIN_TEST(basic_test_int_minimum_stack)
    nm::advcpp::MinimumStack<int> minStack;

    minStack.Push(5);
    ASSERT_THAT(minStack.Minimum() == 5);

    minStack.Push(2);
    ASSERT_THAT(minStack.Minimum() == 2);

    minStack.Push(4);
    ASSERT_THAT(minStack.Minimum() == 2);

    minStack.Pop();
    ASSERT_THAT(minStack.Minimum() == 2);

    minStack.Pop();
    ASSERT_THAT(minStack.Minimum() == 5);

    minStack.Push(1);
    minStack.Push(2);
    minStack.Push(3);
    ASSERT_THAT(minStack.Top() == 3);
    ASSERT_THAT(minStack.Minimum() == 1);
    ASSERT_THAT(minStack.Minimum() == 1); // To check that the minimum did not pop out for some reason...
END_TEST


BEGIN_TEST(throw_exception_test_int_minimum_stack)
    nm::advcpp::MinimumStack<int> minStack;

    try
    {
        minStack.Minimum();
        ASSERT_FAIL("Exception should have thrown by now");
    }
    catch (const std::exception& e)
    {
    }

    try
    {
        minStack.Top();
        ASSERT_FAIL("Exception should have thrown by now");
    }
    catch (const std::exception& e)
    {
    }

    try
    {
        minStack.Pop();
        ASSERT_FAIL("Exception should have thrown by now");
    }
    catch (const std::exception& e)
    {
    }

    ASSERT_PASS();
END_TEST


BEGIN_TEST(check_double_push_of_same_min_element_int_minimum_stack)
    nm::advcpp::MinimumStack<int> minStack;

    minStack.Push(5);
    minStack.Push(2);
    minStack.Push(2);
    ASSERT_THAT(minStack.Minimum() == 2);
    ASSERT_THAT(minStack.Pop() == 2);
    ASSERT_THAT(minStack.Minimum() == 2); // Minimum is still 2

END_TEST


BEGIN_TEST(basic_test_ball_minimum_stack)
    using nm::advcpp::Ball;

    nm::advcpp::MinimumStack<Ball> minStack;

    minStack.Push(Ball(5, Ball::BLUE));
    ASSERT_EQUAL(minStack.Minimum(), Ball(5, Ball::BLUE));

    minStack.Push(Ball(2, Ball::GREEN));
    ASSERT_THAT(minStack.Minimum() == Ball(2, Ball::GREEN));

    minStack.Push(Ball(4, Ball::RED));
    ASSERT_THAT(minStack.Minimum() == Ball(2, Ball::GREEN));

    minStack.Pop();
    ASSERT_THAT(minStack.Minimum() == Ball(2, Ball::GREEN));

    minStack.Pop();
    ASSERT_EQUAL(minStack.Minimum(), Ball(5, Ball::BLUE));
END_TEST


BEGIN_TEST(large_amount_of_push_balls)
    using nm::advcpp::Ball;
    nm::advcpp::MinimumStack<Ball> minStack;
    constexpr size_t N = 1000000;

    std::array<size_t, N> radiusMappingArray;
    for(size_t i = 0; i < radiusMappingArray.size(); ++i)
    {
        radiusMappingArray[i] = i + 1; // radius 0 is not valid
    }
    std::vector<Ball> blackBallsCollection(radiusMappingArray.size());
    std::transform(radiusMappingArray.begin(), radiusMappingArray.end(), blackBallsCollection.begin(), [](const size_t a_radius){ return nm::advcpp::Ball(a_radius, nm::advcpp::Ball::BLACK); });

    for(size_t i = 0; i < blackBallsCollection.size(); ++i)
    {
        minStack.Push(blackBallsCollection.at(i));
    }

    ASSERT_THAT(minStack.Minimum() == Ball(1, Ball::BLACK));
    ASSERT_THAT(minStack.Top() == Ball(blackBallsCollection.size(), Ball::BLACK));

END_TEST


BEGIN_SUITE(MinimumStackTest)

    // int MinimumStack
    TEST(basic_test_int_minimum_stack)
    TEST(throw_exception_test_int_minimum_stack)
    TEST(check_double_push_of_same_min_element_int_minimum_stack)

    // Ball MinimumStack
    TEST(basic_test_ball_minimum_stack)
    TEST(large_amount_of_push_balls)

END_SUITE
