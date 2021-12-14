/**
 * @file Power.hpp
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Metaprogramming Power implementation
 * @version 1.0
 * @date 2021-06-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NM_META_POWER_HPP__
#define __NM_META_POWER_HPP__


namespace nm {

namespace meta {

template <unsigned long Base, unsigned short Exponent>
struct Power {
    static constexpr unsigned long Value = Base * Power<Base, Exponent - 1>::Value;
};


// Specialization of the base case
template <unsigned long Base>
struct Power<Base, 1> {
    static constexpr unsigned long Value = Base;
};

// template <unsigned long Base>
// struct Power<Base, 0> {
//     static constexpr unsigned long Value = 1;
// };

} // meta

} //nm


#endif // __NM_META_POWER_HPP__