#ifndef SF_POINT_HPP
#define SF_POINT_HPP


#include <iostream> // std::cout
#include <string>
#include "iser_fw_types.hpp"
#include "sf_base.hpp"


namespace ser_fw_test
{

class Point : public Base
{
public:
    Point() = default;
    Point(const std::string& a_name, double a_x, double a_y);

    virtual ser_fw::Types::ParamsMap TransformTo() const override;
    virtual void TransformFrom(const ser_fw::Types::ParamsMap& a_paramsMap) override;
    virtual std::string TypeName() const override { return "Point"; }

    // For visual testing:
    void Print() const { std::cout <<  "Point " << m_name << ": " << "(" << m_x << ", " << m_y << ")\n"; }
    double X() const { return m_x; }
    double Y() const { return m_y; }

private:
    double m_x;
    double m_y;
};

} // ser_fw_test


#endif // SF_POINT_HPP
