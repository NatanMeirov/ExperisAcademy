#ifndef NM_MATRIX_HPP
#define NM_MATRIX_HPP


#include <cstddef> // size_t
#include <type_traits> // std::enable_if, std::is_arithmetic
#include "nm_matrix_iterator.hpp"
#include "nm_row_proxy.hpp"


namespace nm
{

// Concept of Ty: Ty must be copy-assignable, copy-constructable, and default-constructable.
// Concept of R (rows) and C (columns): must be greater than 0.
// Tyo be able to use the most of the operations of the Matrix - Ty must implement (overload) some operators:
// +, +=, -, -=, *, *=, /, /=, and etc...
template <typename Ty, size_t R, size_t C>
class Matrix
{
public:
    typedef MatrixIterator<Ty> iterator;
    typedef ConstMatrixIterator<Ty> const_iterator;

public:
    Matrix<Ty,R,C>();
    explicit Matrix<Ty,R,C>(const Ty& a_initialValue);
    Matrix<Ty,R,C>(const Matrix<Ty,R,C>& a_other);
    Matrix<Ty,R,C>& operator=(const Matrix<Ty,R,C>& a_other);
    Matrix<Ty,R,C>(Matrix<Ty,R,C>&& a_rvalue) noexcept; // Move semantics
    Matrix<Ty,R,C>& operator=(Matrix<Ty,R,C>&& a_rvalue) noexcept; // Move semantics
    ~Matrix<Ty,R,C>();

    iterator begin() { return iterator(Begin()); }
    iterator end() { return iterator(End()); }
    const_iterator begin() const { return const_iterator(Begin()); }
    const_iterator end() const { return const_iterator (End()); }

    size_t Rows() const { return R; }
    size_t Columns() const { return C; }

    RowProxy<Ty> operator[](size_t a_rowIndex);
    ConstRowProxy<Ty> operator[](size_t a_rowIndex) const;

    Matrix<Ty,R,C> operator-() const;
    Matrix<Ty,R,C> operator+(const Matrix<Ty,R,C>& a_other) const;
    Matrix<Ty,R,C> operator-(const Matrix<Ty,R,C>& a_other) const;

    template<Ty, size_t ROWS, size_t COLUMNS>
    Matrix<Ty,R,COLUMNS> operator*(const Matrix<Ty,ROWS,COLUMNS>& a_other) const;

    Matrix<Ty,R,C> operator+(const Ty& a_scalar) const;
    Matrix<Ty,R,C> operator-(const Ty& a_scalar) const;
    Matrix<Ty,R,C> operator*(const Ty& a_scalar) const;
    Matrix<Ty,R,C> operator/(const Ty& a_scalar) const;

    Matrix<Ty,R,C>& operator+=(const Matrix<Ty,R,C>& a_other);
    Matrix<Ty,R,C>& operator-=(const Matrix<Ty,R,C>& a_other);
    Matrix<Ty,R,C>& operator+=(const Ty& a_scalar);
    Matrix<Ty,R,C>& operator-=(const Ty& a_scalar);
    Matrix<Ty,R,C>& operator*=(const Ty& a_scalar);
    Matrix<Ty,R,C>& operator/=(const Ty& a_scalar);

    bool operator==(const Matrix<Ty,R,C>& a_other) const;
    bool operator!=(const Matrix<Ty,R,C>& a_other) const;

    void Fill(const Ty& a_value);
    Matrix<Ty,C,R> RotateLeft() const;
    Matrix<Ty,C,R> RotateRight() const;

    // Math:
    Matrix<Ty,C,R> T() const; // Transpose matrix (Mathematical term)
    Matrix<Ty,C,R> Transpose() const; // Transpose matrix

    template <typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type = true>
    Matrix<Ty,R,C> O() const; // Zero matrix (Mathematical term)
    template <typename std::enable_if<std::is_arithmetic<Ty>::value, bool>::type = true>
    Matrix<Ty,R,C> Zero() const; // Zero matrix
    template <typename std::enable_if<R == C && std::is_arithmetic<Ty>::value, bool>::type = true> // Enable only on square matrix
    Matrix<Ty,R,R> I() const; // Identity matrix (Mathematical term)
    template <typename std::enable_if<R == C && std::is_arithmetic<Ty>::value, bool>::type = true> // Enable only on square matrix
    Matrix<Ty,R,R> Identity() const; // Identity matrix

private:
    Ty* Begin() { return m_underlyingArray; }
    Ty* End() { return m_underlyingArray + (R * C); }

private:
    Ty* m_underlyingArray;
};

} // nm


#include "nm_matrix.hxx"


#endif // NM_MATRIX_HPP
