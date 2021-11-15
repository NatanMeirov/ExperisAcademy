#ifndef NM_MATRIX_ITERATOR_HPP
#define NM_MATRIX_ITERATOR_HPP


#include <cstddef> // size_t


namespace nm
{

namespace compiletime
{

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


} // compiletime

} // nm


#include "nm_matrix_iterator.hxx"


#endif // NM_MATRIX_ITERATOR_HPP
