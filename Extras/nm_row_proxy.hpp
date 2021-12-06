#ifndef NM_ROW_PROXY_HPP
#define NM_ROW_PROXY_HPP


#include <cstddef> // size_t
#include <stdexcept> // std::runtime_error


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

    T& operator[](size_t a_index)
    {
        if(a_index >= m_rowSize)
        {
            throw std::overflow_error("Index out of bounds error");
        }

        return m_row[a_index];
    }

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

    const T& operator[](size_t a_index) const
    {
        if(a_index >= m_rowSize)
        {
            throw std::overflow_error("Index out of bounds error");
        }

        return m_row[a_index];
    }

private:
    const T* m_row;
    size_t m_rowSize;
};

} // nm


#endif // NM_ROW_PROXY_HPP
