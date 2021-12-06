#include "filling_indicator.hpp"
#include "point.hpp"


namespace nm
{

	FillingIndicator::FillingIndicator(const Point<>& a_point, FillingIndicator::Direction a_direction, FillingIndicator::Side a_side)
	: m_point(a_point), m_direction(a_direction), m_side(a_side)
	{
	}


	const Point<>& FillingIndicator::Coordinates() const
	{
		return m_point;
	}


	FillingIndicator::Direction FillingIndicator::FillingDirection() const
	{
		return m_direction;
	}


	FillingIndicator::Side FillingIndicator::FillingSide() const
	{
		return m_side;
	}

} // nm
