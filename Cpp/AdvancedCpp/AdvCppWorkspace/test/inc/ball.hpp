#ifndef NM_BALL_HPP
#define NM_BALL_HPP


#include <cstddef> // size_t
#include <iostream> // std::ostream


namespace nm
{

namespace advcpp
{

class Ball
{
    friend std::ostream& operator<<(std::ostream& a_os, const Ball& a_ball);
public:
    enum Color {BLACK, WHITE, BLUE, RED, YELLOW, GREEN, LIGHTBLUE, PURPLE, PINK};

    Ball() : m_radius(1), m_color(Color::BLACK) {}
    Ball(const size_t a_radius, const Color a_color); // Throws if radius == 0
    Ball(const Ball& a_other) = default;
    Ball& operator=(const Ball& a_other) = default;
    ~Ball() = default;

    bool operator<(const Ball& a_other) const;
    bool operator==(const Ball& a_other) const;

private:
    static constexpr const char* COLORS_MAP[]
    {
        "Blue", "Red", "Yellow", "Green", "Lightblue", "Black", "White", "Purple", "Pink"
    };

    size_t m_radius;
    Color m_color;
};

} // advcpp

} // nm


#endif // NM_BALL_HPP