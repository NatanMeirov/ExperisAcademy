#ifndef SF_BASE_HPP
#define SF_BASE_HPP


#include <string>
#include "iser_fw_obj.hpp"


namespace ser_fw_test
{

class Base : public ser_fw::infra::ISerFwObj
{
public:
    virtual void Print() const = 0;
protected:
    Base() = default;
    Base(const std::string& a_name) : m_name(a_name) {}

protected:
    std::string m_name;
};

} // ser_fw_test


#endif // SF_BASE_HPP
