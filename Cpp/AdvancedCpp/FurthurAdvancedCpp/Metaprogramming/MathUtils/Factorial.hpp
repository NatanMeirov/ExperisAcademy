/**
 * @file Factorial.hpp
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Metaprogramming Factorial implementation
 * @version 1.0
 * @date 2021-06-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NM_META_FACTORIAL_HPP__
#define __NM_META_FACTORIAL_HPP__


namespace nm {

namespace meta {

template <unsigned long N>
struct Factorial {
    static constexpr unsigned long Value = N * Factorial<N - 1>::Value;
};


// Specialization of the base case
template <>
struct Factorial<0> {
    static constexpr unsigned long Value = 1;
};

} // meta

} //nm


#endif // __NM_META_FACTORIAL_HPP__