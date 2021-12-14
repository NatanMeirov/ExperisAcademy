#define WHALEDATA


#include "Whale.hpp"
#include "GlobalAnimalsData.hpp"


AnimalData Whale::m_whaleAnimalData(g_whaleLifeExpectency, g_whaleContinent, g_whaleFood, g_whaleSpeed);
MammalData Whale::m_whaleMammalData(g_whalePregnancyDuration, g_whaleBornInLitter);
FishData Whale::m_whaleFishData(g_whaleLowestDepth);


void Whale::Print(std::ostream& a_os) const
{
    this->Animal::Print(a_os);
	this->m_whaleMammalEntity->Print(a_os);
	this->m_whaleFishEntity->Print(a_os);
}
