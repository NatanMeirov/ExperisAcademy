#ifndef NM_ROW_PROXY_HPP
#define NM_ROW_PROXY_HPP


#include <cstddef> // size_t


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

} // nm


#include "nm_row_proxy.hxx"


#endif // NM_ROW_PROXY_HPP
