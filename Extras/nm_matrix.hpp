#ifndef NM_MATRIX_HPP
#define NM_MATRIX_HPP


#include <cstddef> // size_t
#include "nm_matrix_iterator.hpp"
#include "nm_row_proxy.hpp"


namespace nm
{

namespace compiletime
{

// Concept of T: T must be copy-assignable, copy-constructable, and default-constructable.
// Concept of R (rows) and C (columns): must be greater than 0.
// To be able to use the most of the operations of the Matrix - T must implement (overload) some operators:
// +, +=, -, -=, *, *=, /, /=, and etc...
template <typename T, size_t R, size_t C>
class Matrix
{
public:
    typedef MatrixIterator<T> iterator;
    typedef ConstMatrixIterator<T> const_iterator;

public:
    Matrix<T,R,C>();
    explicit Matrix<T,R,C>(const T& a_initialValue);
    Matrix<T,R,C>(const Matrix<T,R,C>& a_other);
    Matrix<T,R,C>& operator=(const Matrix<T,R,C>& a_other);
    Matrix<T,R,C>(Matrix<T,R,C>&& a_rvalue) noexcept; // Move semantics
    Matrix<T,R,C>& operator=(Matrix<T,R,C>&& a_rvalue) noexcept; // Move semantics
    ~Matrix<T,R,C>();

    iterator begin() { return iterator(Begin()); }
    iterator end() { return iterator(End()); }
    const_iterator begin() const { return const_iterator(Begin()); }
    const_iterator end() const { return const_iterator (End()); }

    size_t Rows() const { return R; }
    size_t Columns() const { return C; }

    RowProxy<T> operator[](size_t a_rowIndex);
    ConstRowProxy<T> operator[](size_t a_rowIndex) const;

    Matrix<T,R,C> operator-() const;
    Matrix<T,R,C> operator+(const Matrix<T,R,C>& a_other) const;
    Matrix<T,R,C> operator-(const Matrix<T,R,C>& a_other) const;

    template<T, size_t ROWS, size_t COLUMNS>
    Matrix<T,R,COLUMNS> operator*(const Matrix<T,ROWS,COLUMNS>& a_other) const;

    Matrix<T,R,C> operator+(const T& a_scalar) const;
    Matrix<T,R,C> operator-(const T& a_scalar) const;
    Matrix<T,R,C> operator*(const T& a_scalar) const;
    Matrix<T,R,C> operator/(const T& a_scalar) const;

    Matrix<T,R,C>& operator+=(const Matrix<T,R,C>& a_other);
    Matrix<T,R,C>& operator-=(const Matrix<T,R,C>& a_other);
    Matrix<T,R,C>& operator+=(const T& a_scalar);
    Matrix<T,R,C>& operator-=(const T& a_scalar);
    Matrix<T,R,C>& operator*=(const T& a_scalar);
    Matrix<T,R,C>& operator/=(const T& a_scalar);

    bool operator==(const Matrix<T,R,C>& a_other) const;
    bool operator!=(const Matrix<T,R,C>& a_other) const;

    void Fill(const T& a_value);

private:
    T* Begin() { return m_underlyingArray; }
    T* End() { return m_underlyingArray + (R * C); }

private:
    T* m_underlyingArray;
};

} // compiletime

} // nm


// #include "nm_matrix.hxx"


#endif // NM_MATRIX_HPP
