#include "ball.hpp"
#include <cstddef> // size_t
#include <iostream> // std::ostream
#include <string> // std::to_string
#include <stdexcept> // std::invalid_argument


nm::advcpp::Ball::Ball(const size_t a_radius, const Color a_color)
: m_radius(a_radius)
, m_color(a_color)
{
    if(!this->m_radius) {
        throw std::invalid_argument("Radius cannot be 0");
    }
}


bool nm::advcpp::Ball::operator<(const Ball &a_other) const
{
    return this->m_radius < a_other.m_radius;
}


bool nm::advcpp::Ball::operator==(const Ball &a_other) const
{
    return this->m_radius == a_other.m_radius && this->m_color == a_other.m_color;
}


namespace nm
{

namespace advcpp
{

constexpr const char* Ball::COLORS_MAP[]; // Definition

std::ostream& operator<<(std::ostream& a_os, const Ball& a_ball)
{
    return a_os << "\n{\n\tBall:\n\t\t{\n\t\t\tradius: " + std::to_string(a_ball.m_radius) + ",\n\t\t\tColor: " + Ball::COLORS_MAP[a_ball.m_color] + "\n\t\t}\n}\n";
}

} // advcpp

} // nm