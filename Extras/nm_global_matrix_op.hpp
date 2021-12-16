#ifndef NM_GLOBAL_MATRIX_OP_HPP
#define NM_GLOBAL_MATRIX_OP_HPP


#include "nm_matrix.hpp"


template <typename T, size_t R, size_t C>
nm::Matrix<T,R,C> operator+(const T& a_scalar, nm::Matrix<T,R,C>& a_matrix)
{
    return a_matrix + a_scalar;
}


template <typename T, size_t R, size_t C>
nm::Matrix<T,R,C> operator-(const T& a_scalar, nm::Matrix<T,R,C>& a_matrix)
{
    return a_matrix - a_scalar;
}


template <typename T, size_t R, size_t C>
nm::Matrix<T,R,C> operator*(const T& a_scalar, nm::Matrix<T,R,C>& a_matrix)
{
    return a_matrix * a_scalar;
}


template <typename T, size_t R, size_t C>
nm::Matrix<T,R,C> operator/(const T& a_scalar, nm::Matrix<T,R,C>& a_matrix)
{
    return a_matrix / a_scalar;
}


#endif // NM_GLOBAL_MATRIX_OP_HPP
