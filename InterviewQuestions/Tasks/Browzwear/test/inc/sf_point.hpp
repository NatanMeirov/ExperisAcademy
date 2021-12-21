#ifndef SF_POINT_HPP
#define SF_POINT_HPP


#include <string>
#include "ser_fw_types.hpp"
#include "sf_base.hpp"


namespace ser_fw
{

class Point : public Base
{
public:
    Point() = default;
    Point(const std::string& a_name, double a_x, double a_y);

    virtual Types::ParamsMap TransformTo() const override;
    virtual void TransformFrom(const Types::ParamsMap& a_paramsMap) override;

private:
    double m_x;
    double m_y;
};

} // ser_fw


#endif // SF_POINT_HPP
