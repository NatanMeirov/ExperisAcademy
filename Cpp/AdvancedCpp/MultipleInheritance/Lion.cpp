#define LIONDATA

#include "Lion.hpp"
#include "GlobalAnimalsData.hpp"


MammalData Lion::_lionData(g_lionPregnancyDuration, g_lionBornInLitter, g_lionLifeExpectency, g_lionsContinent, g_lionsFood, g_lionSpeed);


void Lion::Print(std::ostream& a_os) const {
	this->Mammal::Print(a_os);
}
