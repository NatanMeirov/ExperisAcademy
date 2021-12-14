#include "mu_test.h"
#include <vector>
#include <cstddef> // size_t
#include "search_alg.hpp"
#include "fillers.hpp"


BEGIN_TEST(RegularBinarySearchTry)
    constexpr size_t ARRAY_LENGTH = 1000000;
    int* testArray = new int[ARRAY_LENGTH];
    int* testArray2 = new int[ARRAY_LENGTH];
    std::vector<int> testVector;
    std::vector<int> testVector2(ARRAY_LENGTH);

    nm::advcpp::Fill(testArray, -90000, 5, ARRAY_LENGTH);
    nm::advcpp::Fill(testVector, -90000, 5, ARRAY_LENGTH);
    nm::advcpp::Fill(testArray2, testArray2 + ARRAY_LENGTH, -90000, 5);
    nm::advcpp::Fill(testVector2.begin(), testVector2.end(), -90000, 5);

    size_t indexArray = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testArray, ARRAY_LENGTH); // index 0
    size_t indexVector = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testVector, ARRAY_LENGTH); // index 0
    size_t indexArray2 = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testArray2, ARRAY_LENGTH); // index 0
    size_t indexVector2 = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testVector2, ARRAY_LENGTH); // index 0

    ASSERT_THAT(indexArray == 0);
    ASSERT_THAT(indexVector == 0);
    ASSERT_THAT(indexArray2 == 0);
    ASSERT_THAT(indexVector2 == 0);

    delete[] testArray;
END_TEST


BEGIN_TEST(RegularFailBinarySearchTry)
    constexpr size_t ARRAY_LENGTH = 1000000;
    int* testArray = new int[ARRAY_LENGTH];
    int* testArray2 = new int[ARRAY_LENGTH];
    std::vector<int> testVector;
    std::vector<int> testVector2(ARRAY_LENGTH);
    testVector2.reserve(ARRAY_LENGTHARRAY_LENGTH); // Allocate enough capacity for the vector but does NOT call the default c'tor, and the push back always would be O(1) [instead of O(1) and sometimes O(n) for reallocation and copy the prev content...]

    nm::advcpp::Fill(testArray, -90000, 5, ARRAY_LENGTH);
    nm::advcpp::Fill(testVector, -90000, 5, ARRAY_LENGTH);
    nm::advcpp::Fill(testArray2, testArray2 + ARRAY_LENGTH, -90000, 5);
    nm::advcpp::Fill(testVector2.begin(), testVector2.end(), -90000, 5);

    size_t indexArray = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testArray, ARRAY_LENGTH); // index 0
    size_t indexVector = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testVector, ARRAY_LENGTH); // index 0
    size_t indexArray2 = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testArray2, ARRAY_LENGTH); // index 0
    size_t indexVector2 = nm::advcpp::SearchAlgorithms::BinarySearch(-90000, testVector2, ARRAY_LENGTH); // index 0

    ASSERT_THAT(indexArray != 0);
    ASSERT_THAT(indexVector != 0);
    ASSERT_THAT(indexArray2 != 0);
    ASSERT_THAT(indexVector2 != 0);

    delete[] testArray;
END_TEST


BEGIN_SUITE(SearchAlgorithmsUnittesting)

    TEST(RegularBinarySearchTry)
    IGNORE_TEST(RegularFailBinarySearchTry)

END_SUITE
