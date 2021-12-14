#ifndef __FISH_HPP__
#define __FISH_HPP__


#include <ostream> // std::ostream
#include "Animal.hpp"
#include "AnimalData.hpp"


//forward declaration
class FishData;


class Fish : Animal
{
public:
    Fish(const std::string& a_name, const std::string& a_species) : Animal(a_name, a_species) {}

	double GetLowestDepth() const;
    virtual void Print(std::ostream& a_os) const;

protected:
    virtual const AnimalData& GetAnimalData() const { return this->m_animalData; };
	virtual const FishData& GetFishData() const = 0;

private:
	double m_lowestDepth;
    static AnimalData m_animalData;
};


#endif // __FISH_HPP__