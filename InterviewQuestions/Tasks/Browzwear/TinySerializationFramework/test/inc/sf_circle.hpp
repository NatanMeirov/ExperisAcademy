#ifndef SF_CIRCLE_HPP
#define SF_CIRCLE_HPP


#include "sf_base.hpp"
#include "sf_point.hpp"


namespace ser_fw_test
{

class Circle : public Point
{
public:
    Circle() = default;
    Circle(const std::string& a_name, double a_x, double a_y, unsigned int a_radius);

    virtual ser_fw::Types::ParamsMap ToParamsMap() const override;
    virtual void FromParamsMap(const ser_fw::Types::ParamsMap& a_paramsMap) override;
    virtual std::string TypeName() const override { return "Circle"; }

    // For visual testing:
    void Print() const override { Point::Print(); std::cout << ", radius: " << m_radius; }
private:
    unsigned int m_radius;
};

} // ser_fw_test


#endif // SF_CIRCLE_HPP
