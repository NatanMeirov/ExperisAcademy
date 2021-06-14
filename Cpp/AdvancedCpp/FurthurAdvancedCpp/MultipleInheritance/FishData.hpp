#ifndef __FISHDATA_HPP__
#define __FISHDATA_HPP__


#include "AnimalData.hpp"


class FishData : AnimalData
{
public:
	FishData(double a_lifeExpectancy, const std::vector<std::string>& a_continents, const std::vector<std::string>& a_food, double a_speed, double a_lowestDepth)
    : AnimalData(a_lifeExpectancy, a_continents, a_food, a_speed), m_lowestDepth(a_lowestDepth) {}

	double m_lowestDepth;
};


#endif // __FISHDATA_HPP__