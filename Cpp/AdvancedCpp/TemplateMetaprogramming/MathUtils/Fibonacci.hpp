/**
 * @file Fibonacci.hpp
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Metaprogramming Fibonacci implementation
 * @version 1.0
 * @date 2021-06-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NM_META_FIBONACCI_HPP__
#define __NM_META_FIBONACCI_HPP__


#include <climits> // CHAR_BIT


namespace nm {

namespace meta {

template <unsigned long N>
struct Fibonacci {
    static constexpr unsigned long Value = Fibonacci<N - 1>::Value + Fibonacci<N - 2>::Value;
};


// Specialization of the base case: N = 0
template <>
struct Fibonacci<1> {
    static constexpr unsigned long Value = 0;
};


// Specialization of the base case: N = 1
template <>
struct Fibonacci<2> {
    static constexpr unsigned long Value = 1;
};

} // meta

} //nm


#endif // __NM_META_FIBONACCI_HPP__