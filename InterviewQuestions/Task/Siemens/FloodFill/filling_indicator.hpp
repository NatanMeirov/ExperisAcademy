#ifndef FILLING_INDICATOR_HPP
#define FILLING_INDICATOR_HPP


#include "point.hpp"


namespace nm
{

    class FillingIndicator
    {
    public:
        enum Direction { Left, Right };
        enum Side { Up, Down };

    public:
        FillingIndicator(const Point<>& a_point, Direction a_direction, Side a_side);
        FillingIndicator(const FillingIndicator& a_other) = default;
        FillingIndicator& operator=(const FillingIndicator& a_other) = default;
        ~FillingIndicator() = default;

        const Point<>& Coordinates() const; // Most of the time, Point<T> is equal or more than 8 bytes, so reference is the best choise here
        Direction FillingDirection() const;
        Side FillingSide() const;

    private:
        Point<> m_point;
        Direction m_direction;
        Side m_side;
    };

} // nm

#endif // FILLING_INDICATOR_HPP
