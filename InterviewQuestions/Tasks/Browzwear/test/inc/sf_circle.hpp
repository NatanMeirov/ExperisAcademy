#ifndef SF_CIRCLE_HPP
#define SF_CIRCLE_HPP


#include "sf_base.hpp"
#include "sf_point.hpp"


namespace ser_fw
{

class Circle : public Point
{
public:
    Circle() = default;
    Circle(const std::string& a_name, double a_x, double a_y, unsigned int a_radius);

    virtual Types::ParamsMap TransformTo() const override;
    virtual void TransformFrom(const Types::ParamsMap& a_paramsMap) override;
    virtual std::string TypeName() const override { return "Circle"; }

private:
    unsigned int m_radius;
};

} // ser_fw


#endif // SF_CIRCLE_HPP
