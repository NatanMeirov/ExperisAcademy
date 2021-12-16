#ifndef NM_MATRIX_HPP
#define NM_MATRIX_HPP


#include <cstddef> // size_t
#include <type_traits> // std::enable_if, std::is_arithmetic
#include <algorithm> // std::copy, std::equal, std::transform, std::fill
#include <stdexcept> // std::overflow_error


namespace nm
{

template<typename T>
class RowProxy
{
public:
    RowProxy(T* a_row, size_t a_rowSize) : m_row(a_row), m_rowSize(a_rowSize) {}
    RowProxy(const RowProxy& a_other) = default;
    RowProxy& operator=(const RowProxy& a_other) = default;
    ~RowProxy() = default;

    T& operator[](size_t a_columnIndex);
    void Fill(const T& a_value);

private:
    T* m_row;
    size_t m_rowSize;
};


template<typename T>
class ConstRowProxy
{
public:
    ConstRowProxy(const T* a_row, size_t a_rowSize) : m_row(a_row), m_rowSize(a_rowSize) {}
    ConstRowProxy(const ConstRowProxy& a_other) = default;
    ConstRowProxy& operator=(const ConstRowProxy& a_other) = default;
    ~ConstRowProxy() = default;

    const T& operator[](size_t a_columnIndex) const;

private:
    const T* m_row;
    size_t m_rowSize;
};


// The MatrixIterator and ConstMatrixIterator iterators are stable bidirectional iterators
template<typename T>
class MatrixIteratorImpl
{
public:
    MatrixIteratorImpl(T* a_matrixElm) : m_matrixElm(a_matrixElm) {}
    MatrixIteratorImpl(const MatrixIteratorImpl<T>& a_other) = default;
    MatrixIteratorImpl& operator=(const MatrixIteratorImpl<T>& a_other) = default;
    ~MatrixIteratorImpl() = default;

    MatrixIteratorImpl<T>& operator++();
    MatrixIteratorImpl<T>& operator--();
    MatrixIteratorImpl<T>& operator+=(size_t a_offset);
    MatrixIteratorImpl<T>& operator-=(size_t a_offset);

    bool operator!=(const MatrixIteratorImpl<T>& a_other);
    bool operator==(const MatrixIteratorImpl<T>& a_other);
    bool operator<=(const MatrixIteratorImpl<T>& a_other);
    bool operator>=(const MatrixIteratorImpl<T>& a_other);
    bool operator<(const MatrixIteratorImpl<T>& a_other);
    bool operator>(const MatrixIteratorImpl<T>& a_other);

    T* MatrixElement();

private:
    T* m_matrixElm;
};


template<typename T>
class MatrixIterator
{
public:
    MatrixIterator(T* a_matrixElm) : m_itr(a_matrixElm) {}
    MatrixIterator(const MatrixIterator<T>& a_other) = default;
    MatrixIterator& operator=(const MatrixIterator<T>& a_other) = default;
    ~MatrixIterator() = default;

    MatrixIterator<T>& operator++();
    MatrixIterator<T>& operator--();
    MatrixIterator<T> operator++(int);
    MatrixIterator<T> operator--(int);
    MatrixIterator<T> operator+(size_t a_offset);
    MatrixIterator<T> operator-(size_t a_offset);
    MatrixIterator<T>& operator+=(size_t a_offset);
    MatrixIterator<T>& operator-=(size_t a_offset);

    bool operator!=(const MatrixIterator<T>& a_other);
    bool operator==(const MatrixIterator<T>& a_other);
    bool operator<=(const MatrixIterator<T>& a_other);
    bool operator>=(const MatrixIterator<T>& a_other);
    bool operator<(const MatrixIterator<T>& a_other);
    bool operator>(const MatrixIterator<T>& a_other);

    T& operator*();

private:
    MatrixIteratorImpl<T> m_itr;
};


template<typename T>
class ConstMatrixIterator
{
public:
    ConstMatrixIterator(T* a_matrixElm) : m_itr(a_matrixElm) {}
    ConstMatrixIterator(const ConstMatrixIterator<T>& a_other) = default;
    ConstMatrixIterator& operator=(const ConstMatrixIterator<T>& a_other) = default;
    ~ConstMatrixIterator() = default;

    ConstMatrixIterator<T>& operator++();
    ConstMatrixIterator<T>& operator--();
    ConstMatrixIterator<T> operator++(int);
    ConstMatrixIterator<T> operator--(int);
    ConstMatrixIterator<T> operator+(size_t a_offset);
    ConstMatrixIterator<T> operator-(size_t a_offset);
    ConstMatrixIterator<T>& operator+=(size_t a_offset);
    ConstMatrixIterator<T>& operator-=(size_t a_offset);

    bool operator!=(const ConstMatrixIterator<T>& a_other);
    bool operator==(const ConstMatrixIterator<T>& a_other);
    bool operator<=(const ConstMatrixIterator<T>& a_other);
    bool operator>=(const ConstMatrixIterator<T>& a_other);
    bool operator<(const ConstMatrixIterator<T>& a_other);
    bool operator>(const ConstMatrixIterator<T>& a_other);

    const T& operator*() const;

private:
    MatrixIteratorImpl<T> m_itr;
};


template<typename T>
MatrixIteratorImpl<T>& MatrixIteratorImpl<T>::operator++()
{
    ++m_matrixElm;
    return *this;
}


template<typename T>
MatrixIteratorImpl<T>& MatrixIteratorImpl<T>::operator--()
{
    --m_matrixElm;
    return *this;
}


template<typename T>
MatrixIteratorImpl<T>& MatrixIteratorImpl<T>::operator+=(size_t a_offset)
{
    m_matrixElm += a_offset;
    return *this;
}


template<typename T>
MatrixIteratorImpl<T>& MatrixIteratorImpl<T>::operator-=(size_t a_offset)
{
    m_matrixElm -= a_offset;
    return *this;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator!=(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm != a_other.m_matrixElm;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator==(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm == a_other.m_matrixElm;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator<=(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm <= a_other.m_matrixElm;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator>=(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm >= a_other.m_matrixElm;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator<(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm < a_other.m_matrixElm;
}


template<typename T>
bool MatrixIteratorImpl<T>::operator>(const MatrixIteratorImpl<T>& a_other)
{
    return m_matrixElm > a_other.m_matrixElm;
}


template<typename T>
T* MatrixIteratorImpl<T>::MatrixElement()
{
    return m_matrixElm;
}


template<typename T>
MatrixIterator<T>& MatrixIterator<T>::operator++()
{
    ++m_itr;
    return *this;
}


template<typename T>
MatrixIterator<T>& MatrixIterator<T>::operator--()
{
    --m_itr;
    return *this;
}


template<typename T>
MatrixIterator<T> MatrixIterator<T>::operator++(int)
{
    MatrixIterator<T> matrixItr(*this);
    ++m_itr;
    return matrixItr;
}


template<typename T>
MatrixIterator<T> MatrixIterator<T>::operator--(int)
{
    MatrixIterator<T> matrixItr(*this);
    --m_itr;
    return matrixItr;
}


template<typename T>
MatrixIterator<T> MatrixIterator<T>::operator+(size_t a_offset)
{
    MatrixIterator<T> matrixItr(*this);
    matrixItr.m_itr += a_offset;
    return matrixItr;
}


template<typename T>
MatrixIterator<T> MatrixIterator<T>::operator-(size_t a_offset)
{
    MatrixIterator<T> matrixItr(*this);
    matrixItr.m_itr -= a_offset;
    return matrixItr;
}



template<typename T>
MatrixIterator<T>& MatrixIterator<T>::operator+=(size_t a_offset)
{
    m_itr += a_offset;
    return *this;
}


template<typename T>
MatrixIterator<T>& MatrixIterator<T>::operator-=(size_t a_offset)
{
    m_itr -= a_offset;
    return *this;
}


template<typename T>
bool MatrixIterator<T>::operator!=(const MatrixIterator<T>& a_other)
{
    return m_itr != a_other.m_itr;
}


template<typename T>
bool MatrixIterator<T>::operator==(const MatrixIterator<T>& a_other)
{
    return m_itr == a_other.m_itr;
}


template<typename T>
bool MatrixIterator<T>::operator<=(const MatrixIterator<T>& a_other)
{
    return m_itr <= a_other.m_itr;
}


template<typename T>
bool MatrixIterator<T>::operator>=(const MatrixIterator<T>& a_other)
{
    return m_itr >= a_other.m_itr;
}


template<typename T>
bool MatrixIterator<T>::operator<(const MatrixIterator<T>& a_other)
{
    return m_itr < a_other.m_itr;
}


template<typename T>
bool MatrixIterator<T>::operator>(const MatrixIterator<T>& a_other)
{
    return m_itr > a_other.m_itr;
}


template<typename T>
T& MatrixIterator<T>::operator*()
{
    return *(m_itr.MatrixElement());
}


template<typename T>
ConstMatrixIterator<T>& ConstMatrixIterator<T>::operator++()
{
    ++m_itr;
    return *this;
}


template<typename T>
ConstMatrixIterator<T>& ConstMatrixIterator<T>::operator--()
{
    --m_itr;
    return *this;
}


template<typename T>
ConstMatrixIterator<T> ConstMatrixIterator<T>::operator++(int)
{
    ConstMatrixIterator<T> constMatrixItr(*this);
    ++m_itr;
    return constMatrixItr;
}


template<typename T>
ConstMatrixIterator<T> ConstMatrixIterator<T>::operator--(int)
{
    ConstMatrixIterator<T> constMatrixItr(*this);
    --m_itr;
    return constMatrixItr;
}


template<typename T>
ConstMatrixIterator<T> ConstMatrixIterator<T>::operator+(size_t a_offset)
{
    ConstMatrixIterator<T> constMatrixItr(*this);
    constMatrixItr.m_itr += a_offset;
    return constMatrixItr;
}


template<typename T>
ConstMatrixIterator<T> ConstMatrixIterator<T>::operator-(size_t a_offset)
{
    ConstMatrixIterator<T> constMatrixItr(*this);
    constMatrixItr.m_itr -= a_offset;
    return constMatrixItr;
}



template<typename T>
ConstMatrixIterator<T>& ConstMatrixIterator<T>::operator+=(size_t a_offset)
{
    m_itr += a_offset;
    return *this;
}


template<typename T>
ConstMatrixIterator<T>& ConstMatrixIterator<T>::operator-=(size_t a_offset)
{
    m_itr -= a_offset;
    return *this;
}


template<typename T>
bool ConstMatrixIterator<T>::operator!=(const ConstMatrixIterator<T>& a_other)
{
    return m_itr != a_other.m_itr;
}


template<typename T>
bool ConstMatrixIterator<T>::operator==(const ConstMatrixIterator<T>& a_other)
{
    return m_itr == a_other.m_itr;
}


template<typename T>
bool ConstMatrixIterator<T>::operator<=(const ConstMatrixIterator<T>& a_other)
{
    return m_itr <= a_other.m_itr;
}


template<typename T>
bool ConstMatrixIterator<T>::operator>=(const ConstMatrixIterator<T>& a_other)
{
    return m_itr >= a_other.m_itr;
}


template<typename T>
bool ConstMatrixIterator<T>::operator<(const ConstMatrixIterator<T>& a_other)
{
    return m_itr < a_other.m_itr;
}


template<typename T>
bool ConstMatrixIterator<T>::operator>(const ConstMatrixIterator<T>& a_other)
{
    return m_itr > a_other.m_itr;
}


template<typename T>
const T& ConstMatrixIterator<T>::operator*() const
{
    return *(m_itr.MatrixElement());
}


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


// Global Matrix Operators:

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


#endif // NM_MATRIX_HPP
