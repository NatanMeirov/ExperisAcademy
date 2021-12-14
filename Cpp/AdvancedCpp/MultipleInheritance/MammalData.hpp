#ifndef __MAMMALDATA_HPP__
#define __MAMMALDATA_HPP__


#include <cstddef> // size_t
#include "AnimalData.hpp"


class MammalData : AnimalData
{
public:
	MammalData(double a_lifeExpectancy, const std::vector<std::string>& a_continents, const std::vector<std::string>& a_food, double a_speed, double a_durationOfPregnancy, size_t a_numOfBornInLitter)
    : AnimalData(a_lifeExpectancy,  a_continents, a_food, a_speed), m_durationOfPregnancy(a_durationOfPregnancy), m_numOfBornInLitter(a_numOfBornInLitter) {}

	double	m_durationOfPregnancy;
	size_t	m_numOfBornInLitter;
};


#endif // __MAMMALDATA_HPP__