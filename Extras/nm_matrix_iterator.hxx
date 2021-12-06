#ifndef NM_MATRIX_ITERATOR_HXX
#define NM_MATRIX_ITERATOR_HXX


#include <cstddef> // size_t


namespace nm
{

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

} // nm


#endif // NM_MATRIX_ITERATOR_HXX
