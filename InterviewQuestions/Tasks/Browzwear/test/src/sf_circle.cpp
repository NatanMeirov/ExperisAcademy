#include "sf_circle.hpp"
#include <string> // std::stof, std::stoul, std::to_string
#include "sf_point.hpp"


namespace ser_fw
{

Circle::Circle(const std::string& a_name, double a_x, double a_y, unsigned int a_radius)
: Point(a_name, a_x, a_y)
, m_radius(a_radius)
{
}


Types::ParamsMap Circle::TransformTo() const
{
    Types::ParamsMap map = Point::TransformTo();
    map.insert({"radius", std::to_string(m_radius)});

    return map;
}


void Circle::TransformFrom(const Types::ParamsMap& a_paramsMap)
{
    *this = Circle(a_paramsMap.at("name"), std::stof(a_paramsMap.at("x")), std::stof(a_paramsMap.at("y")), std::stoul(a_paramsMap.at("radius")));
}

} // ser_fw
