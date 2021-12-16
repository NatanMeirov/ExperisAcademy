#ifndef NM_MATRIX_HXX
#define NM_MATRIX_HXX


#include <cstddef> // size_t
#include <type_traits> // std::enable_if, std::is_arithmetic
#include <algorithm> // std::copy, std::equal, std::transform, std::fill
#include <stdexcept> // std::overflow_error
#include "nm_row_proxy.hpp"
#include "nm_matrix.hpp" //! remove

namespace nm
{

template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>::Matrix()
: m_underlyingArray(new Ty[R * C])
{
    static_assert(R != 0 && C != 0, "R (rows) and C (columns) must be greater than 0");
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>::Matrix(const Ty& a_initialValue)
: Matrix()
{
    Fill(a_initialValue);
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>::Matrix(const Matrix<Ty,R,C>& a_other)
: m_underlyingArray(new Ty[R * C])
{
    std::copy(a_other.Begin(), a_other.End(), Begin());
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator=(const Matrix<Ty,R,C>& a_other)
{
    if(*this == a_other)
    {
        return *this;
    }

    Ty* newUnderlyingArray = new Ty[R * C]; // For exception safety
    std::copy(a_other.Begin(), a_other.End(), newUnderlyingArray);

    delete[] m_underlyingArray;
    m_underlyingArray = newUnderlyingArray;

    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>::Matrix(Matrix<Ty,R,C>&& a_rvalue) noexcept
: m_underlyingArray(a_rvalue.m_underlyingArray)
{
    a_rvalue.m_underlyingArray = nullptr;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator=(Matrix<Ty,R,C>&& a_rvalue) noexcept
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


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>::~Matrix()
{
    delete[] m_underlyingArray;
}


template <typename Ty, size_t R, size_t C>
RowProxy<Ty> Matrix<Ty,R,C>::operator[](size_t a_rowIndex)
{
    if(a_rowIndex >= R)
    {
        throw std::overflow_error("Row index out of bounds error");
    }

    return RowProxy<Ty>(m_underlyingArray + (a_rowIndex * C), C);
}


template <typename Ty, size_t R, size_t C>
ConstRowProxy<Ty> Matrix<Ty,R,C>::operator[](size_t a_rowIndex) const
{
    if(a_rowIndex >= R)
    {
        throw std::overflow_error("Row index out of bounds error");
    }

    return ConstRowProxy<Ty>(m_underlyingArray + (a_rowIndex * C), C);
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator-() const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [](const Ty& a_value){ return -a_value; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator+(const Matrix<Ty,R,C>& a_other) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), a_other.Begin(), matrix.Begin(), [](const Ty& a_firstVal, const Ty& a_secondVal){ return a_firstVal + a_secondVal; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator-(const Matrix<Ty,R,C>& a_other) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), a_other.Begin(), matrix.Begin(), [](const Ty& a_firstVal, const Ty& a_secondVal){ return a_firstVal - a_secondVal; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
template<Ty, size_t ROWS, size_t COLUMNS>
Matrix<Ty,R,COLUMNS> Matrix<Ty,R,C>::operator*(const Matrix<Ty,ROWS,COLUMNS>& a_other) const
{
    static_assert(C == ROWS, "C (columns) of the first matrix must be equal to the ROWS of the second matrix, in matrix multiplication");

    Matrix<Ty,R,COLUMNS> matrix;

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


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator+(const Ty& a_scalar) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const Ty& a_value){ return a_value + a_scalar; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator-(const Ty& a_scalar) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const Ty& a_value){ return a_value - a_scalar; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator*(const Ty& a_scalar) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const Ty& a_value){ return a_value * a_scalar; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C> Matrix<Ty,R,C>::operator/(const Ty& a_scalar) const
{
    Matrix<Ty,R,C> matrix;
    std::transform(Begin(), End(), matrix.Begin(), [a_scalar](const Ty& a_value){ return a_value / a_scalar; });
    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator+=(const Matrix<Ty,R,C>& a_other)
{
    std::transform(Begin(), End(), a_other.Begin(), Begin(), [](const Ty& a_firstVal, const Ty& a_secondVal){ return a_firstVal + a_secondVal; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator-=(const Matrix<Ty,R,C>& a_other)
{
    std::transform(Begin(), End(), a_other.Begin(), Begin(), [](const Ty& a_firstVal, const Ty& a_secondVal){ return a_firstVal - a_secondVal; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator+=(const Ty& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const Ty& a_value){ return a_value + a_scalar; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator-=(const Ty& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const Ty& a_value){ return a_value - a_scalar; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator*=(const Ty& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const Ty& a_value){ return a_value * a_scalar; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,R,C>& Matrix<Ty,R,C>::operator/=(const Ty& a_scalar)
{
    std::transform(Begin(), End(), Begin(), [a_scalar](const Ty& a_value){ return a_value / a_scalar; });
    return *this;
}


template <typename Ty, size_t R, size_t C>
bool Matrix<Ty,R,C>::operator==(const Matrix<Ty,R,C>& a_other) const
{
    return std::equal(Begin(), End(), a_other.Begin());
}


template <typename Ty, size_t R, size_t C>
bool Matrix<Ty,R,C>::operator!=(const Matrix<Ty,R,C>& a_other) const
{
    return !std::equal(Begin(), End(), a_other.Begin());
}


template <typename Ty, size_t R, size_t C>
void Matrix<Ty,R,C>::Fill(const Ty& a_value)
{
    std::fill(Begin(), End(), a_value);
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,C,R> Matrix<Ty,R,C>::Transpose() const
{
    Matrix<Ty,C,R> matrix;

    for(size_t i = 0; i < R; ++i)
    {
        for(size_t j = 0; j < C; ++j)
        {
            matrix.m_underlyingArray[(j * R) + i] = m_underlyingArray[(i * C) + j];
        }
    }

    return matrix;
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,C,R> Matrix<Ty,R,C>::T() const
{
    return Transpose();
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,C,R> Matrix<Ty,R,C>::RotateLeft() const
{
    // TODO
}


template <typename Ty, size_t R, size_t C>
Matrix<Ty,C,R> Matrix<Ty,R,C>::RotateRight() const
{
    // TODO
}


template <typename Ty, size_t R, size_t C>
template <typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type>
Matrix<Ty,R,C> Matrix<Ty,R,C>::O() const
{
    return Zero();
}


template <typename Ty, size_t R, size_t C>
template <typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type>
Matrix<Ty,R,C> Matrix<Ty,R,C>::Zero() const
{
    return Matrix<Ty,R,C>();
}


template <typename Ty, size_t R, size_t C>
template <typename std::enable_if<R == C && std::is_arithmetic<Ty>::value, bool>::type>
Matrix<Ty,R,R> Matrix<Ty,R,C>::Identity() const
{
    Matrix<Ty,R,R> identityMatrix;

    for(size_t i = 0; i < R; ++i)
    {
        identityMatrix.m_underlyingArray[(i * R) + i] = 1;
    }

    return identityMatrix;
}


template <typename Ty, size_t R, size_t C>
template <typename std::enable_if<R == C && std::is_arithmetic<Ty>::value, bool>::type>
Matrix<Ty,R,R> Matrix<Ty,R,C>::I() const
{
    return Identity();
}

} // nm


#endif // NM_MATRIX_HXX
