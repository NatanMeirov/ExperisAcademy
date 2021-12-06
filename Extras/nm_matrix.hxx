#ifndef NM_MATRIX_HXX
#define NM_MATRIX_HXX


#include <cstddef> // size_t
#include <algorithm> // std::copy
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
Matrix<T,R,C>::Matrix(const Matrix<T,R,C>& a_other)
: m_underlyingArray(new T[R * C])
{
    std::copy(a_other.m_underlyingArray, a_other.m_underlyingArray + (R * C), m_underlyingArray);
}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator=(const Matrix<T,R,C>& a_other)
{
    if(*this == a_other)
    {
        return *this;
    }

    T* newUnderlyingArray = new T[R * C]; // For exception safety
    std::copy(a_other.m_underlyingArray, a_other.m_underlyingArray + (R * C), newUnderlyingArray);

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

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator+(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator-(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator+(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator-(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator*(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C> Matrix<T,R,C>::operator/(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
template<T, size_t ROWS, size_t COLUMNS>
Matrix<T, R, COLUMNS> Matrix<T,R,C>::operator*(const Matrix<T,ROWS,COLUMNS>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator+=(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator-=(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator*=(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator+=(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator-=(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator*=(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
Matrix<T,R,C>& Matrix<T,R,C>::operator/=(const T& a_value) const
{

}


template <typename T, size_t R, size_t C>
bool Matrix<T,R,C>::operator==(const Matrix<T,R,C>& a_other) const
{

}


template <typename T, size_t R, size_t C>
bool Matrix<T,R,C>::operator!=(const Matrix<T,R,C>& a_other) const
{

}

} // compiletime

} // nm


#endif // NM_MATRIX_HXX
