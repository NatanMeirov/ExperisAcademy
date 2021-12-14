#ifndef __ANIMALDATA_HPP__
#define __ANIMALDATA_HPP__


#include <vector>
#include <string>


class AnimalData
{
public:
	AnimalData(double a_lifeExpectancy, const std::vector<std::string>& a_continents, const std::vector<std::string>& a_food, double a_speed) : m_lifeExpectancy(a_lifeExpectancy), m_continents(a_continents), m_food(a_food), m_speed(a_speed) {}

	double	m_lifeExpectancy;
	std::vector<std::string> m_continents;
	std::vector<std::string> m_food;
	double	m_speed;
};


#endif // __ANIMALDATA_HPP__