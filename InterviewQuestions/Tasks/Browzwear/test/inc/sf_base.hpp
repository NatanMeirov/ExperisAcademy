#ifndef SF_BASE_HPP
#define SF_BASE_HPP


#include <string>
#include "ser_fw_obj.hpp"


namespace ser_fw
{

class Base : public infra::SerFwObj
{
protected:
    Base() = default;
    Base(const std::string& a_name);

protected:
    std::string m_name;
};

} // ser_fw


#endif // SF_BASE_HPP
