#ifndef __WHALE_HPP__
#define __WHALE_HPP__


#include "Animal.hpp"
#include "Mammal.hpp"
#include "Fish.hpp"
#include "AnimalData.hpp"
#include "MammalData.hpp"
#include "FishData.hpp"


class Whale : public Animal
{
public:
	Whale(const std::string& a_name, const std::string& a_species) :
		Animal(a_name, a_species), m_whaleMammalEntity(new Mammal(a_name, a_species)), m_whaleFishEntity(new Fish(a_name, a_species)) {}


protected:
	virtual const AnimalData& GetAnimalData() const { return this->m_whaleAnimalData; }
	virtual const MammalData& GetMammalData() const { return this->m_whaleMammalData; }
	virtual const FishData& GetFishData() const { return this->m_whaleFishData; }

	virtual void Print(std::ostream& out) const;

private:
    Mammal* m_whaleMammalEntity;
    Fish* m_whaleFishEntity;
	static AnimalData m_whaleAnimalData;
	static MammalData m_whaleMammalData;
	static FishData   m_whaleFishData;

};


#endif // __WHALE_HPP__