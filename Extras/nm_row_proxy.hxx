#ifndef NM_ROW_PROXY_HXX
#define NM_ROW_PROXY_HXX


#include <stdexcept> // std::overflow_error
#include <algorithm> // std::fill


namespace nm
{

template <typename T>
T& RowProxy<T>::operator[](size_t a_columnIndex)
{
    if(a_columnIndex >= m_rowSize)
    {
        throw std::overflow_error("Column index out of bounds error");
    }

    return m_row[a_columnIndex];
}


template <typename T>
void RowProxy<T>::Fill(const T& a_value)
{
    std::fill(m_row, m_row + m_rowSize, a_value);
}


template <typename T>
const T& ConstRowProxy<T>::operator[](size_t a_columnIndex) const
{
    if(a_columnIndex >= m_rowSize)
    {
        throw std::overflow_error("Column index out of bounds error");
    }

    return m_row[a_columnIndex];
}

} // nm


#endif // NM_ROW_PROXY_HXX
