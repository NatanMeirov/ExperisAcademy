#ifndef NM_MATRIX_HXX
#define NM_MATRIX_HXX


#include <cstddef> // size_t
#include <algorithm> // std::copy, std::equal, std::transform, std::fill
#include <stdexcept> // std::overflow_error
#include "nm_row_proxy.hpp"
#include "nm_matrix.hpp" //! remove

namespace nm
{

namespace compiletime
{

template <typename T, size_t R, size_t C>
Matrix<T,R,C>::Matrix()
: m_underlyingArray(new T[R * C])
{
    static_assert(R != 0 && C != 0, "R (rows) and C (columns) must be greater than 0");
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>::Matrix(const T& a_initialValue)
: Matrix()
{
    Fill(a_initialValue);
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>::Matrix(const Matrix<T,R,C>& a_other)
: m_underlyingArray(new T[R * C])
{
    std::copy(a_other.Begin(), a_other.End(), Begin());
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator=(const Matrix<T,R,C>& a_other)
{
    if(*this == a_other)
    {
        return *this;
    }

    T* newUnderlyingArray = new T[R * C]; // For exception safety
    std::copy(a_other.Begin(), a_other.End(), newUnderlyingArray);

    delete[] m_underlyingArray;
    m_underlyingArray = newUnderlyingArray;

    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>::Matrix(Matrix<T,R,C>&& a_rvalue) noexcept
: m_underlyingArray(a_rvalue.m_underlyingArray)
{
    a_rvalue.m_underlyingArray = nullptr;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator=(Matrix<T,R,C>&& a_rvalue) noexcept
{
    if(*this == a_rvalue)
    {
        return *this;
    }

    delete[] m_underlyingArray;
    m_underlyingArray = a_rvalue.m_underlyingArray;
    a_rvalue.m_underlyingArray = nullptr;

    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>::~Matrix()
{
    delete[] m_underlyingArray;
}


template <typename T, size_t R, size_t C>
RowProxy<T> Matrix<T,R,C>::operator[](size_t a_rowIndex)
{
    if(a_rowIndex >= R)
    {
        throw std::overflow_error("Row index out of bounds error");
    }

    return RowProxy<T>(m_underlyingArray + (a_rowIndex * C), C);
}


template <typename T, size_t R, size_t C>
ConstRowProxy<T> Matrix<T,R,C>::operator[](size_t a_rowIndex) const
{
    if(a_rowIndex >= R)
    {
        throw std::overflow_error("Row index out of bounds error");
    }

    return ConstRowProxy<T>(m_underlyingArray + (a_rowIndex * C), C);
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator-() const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [](const T& a_value){ return -a_value; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator+(const Matrix<T,R,C>& a_other) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), a_other.Begin(), matrix.Begin(), [](const T& a_firstVal, const T& a_secondVal){ return a_firstVal + a_secondVal; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator-(const Matrix<T,R,C>& a_other) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), a_other.Begin(), matrix.Begin(), [](const T& a_firstVal, const T& a_secondVal){ return a_firstVal - a_secondVal; });
    return matrix;
}


template <typename T, size_t R, size_t C>
template<T, size_t ROWS, size_t COLUMNS>
Matrix<T,R,COLUMNS> Matrix<T,R,C>::operator*(const Matrix<T,ROWS,COLUMNS>& a_other) const
{
    static_assert(C == ROWS, "C (columns) of the first matrix must be equal to the ROWS of the second matrix, in matrix multiplication");

    Matrix<T,R,COLUMNS> matrix;

    for(size_t i = 0; i < R; ++i)
    {
        for(size_t j = 0; j < COLUMNS; ++j)
        {
            for(size_t k = 0; k < C; ++k)
            {
                matrix.m_underlyingArray[(i * COLUMNS) + j] += m_underlyingArray[(i * C) + k] * a_other.m_underlyingArray[(k * COLUMNS) + j];
            }
        }
    }

    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator+(const T& a_scalar) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const T& a_value){ return a_value + a_scalar; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator-(const T& a_scalar) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const T& a_value){ return a_value - a_scalar; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator*(const T& a_scalar) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const T& a_value){ return a_value * a_scalar; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator/(const T& a_scalar) const
{
    Matrix<T,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const T& a_value){ return a_value / a_scalar; });
    return matrix;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator+=(const Matrix<T,R,C>& a_other)
{
    std::transform(Begin(), End(), a_other.Begin(), Begin(), [](const T& a_firstVal, const T& a_secondVal){ return a_firstVal + a_secondVal; });
    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator-=(const Matrix<T,R,C>& a_other)
{
    std::transform(Begin(), End(), a_other.Begin(), Begin(), [](const T& a_firstVal, const T& a_secondVal){ return a_firstVal - a_secondVal; });
    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator+=(const T& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const T& a_value){ return a_value + a_scalar; });
    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator-=(const T& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const T& a_value){ return a_value - a_scalar; });
    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator*=(const T& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const T& a_value){ return a_value * a_scalar; });
    return *this;
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator/=(const T& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const T& a_value){ return a_value / a_scalar; });
    return *this;
}


template <typename T, size_t R, size_t C>
bool Matrix<T,R,C>::operator==(const Matrix<T,R,C>& a_other) const
{
    return std::equal(Begin(), End(), a_other.Begin());
}


template <typename T, size_t R, size_t C>
bool Matrix<T,R,C>::operator!=(const Matrix<T,R,C>& a_other) const
{
    return !std::equal(Begin(), End(), a_other.Begin());
}


template <typename T, size_t R, size_t C>
void Matrix<T,R,C>::Fill(const T& a_value)
{
    std::fill(Begin(), End(), a_value);
}

} // compiletime

} // nm


#endif // NM_MATRIX_HXX
