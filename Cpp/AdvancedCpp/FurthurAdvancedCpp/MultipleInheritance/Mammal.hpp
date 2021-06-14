#ifndef __MAMMAL_HPP__
#define __MAMMAL_HPP__


#include "Animal.hpp"
#include "AnimalData.hpp"


//forward declaration
class MammalData;


class Mammal : Animal
{
public:
	Mammal(const std::string& a_name, const std::string& a_species) : Animal(a_name, a_species) {}

	double GetDurationOfPregnancy() const;
	size_t GetNumOfBornInLitter() const;
    virtual void Print(std::ostream& a_os) const;

protected:
	virtual const MammalData& GetMammalData() const = 0;
    virtual const AnimalData& GetAnimalData() const { return this->m_animalData; }

private:
    static AnimalData m_animalData;
};


#endif // __MAMMAL_HPP__