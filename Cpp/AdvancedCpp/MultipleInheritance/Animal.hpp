#ifndef __ANIMAL_HPP__
#define __ANIMAL_HPP__


#include <string>
#include <vector>
#include <ostream> // std::ostream


//forward declaration
class AnimalData;


class Animal
{
    friend std::ostream& operator<<(std::ostream& a_os, const Animal& a_animal);
public:
	Animal(const std::string& a_name, const std::string& a_species);

	std::string GetName() const;
	std::string GetSpecies() const;
	double GetLifeExpectancy() const;
	std::vector<std::string> GetContinents() const;
	std::vector<std::string> GetFood() const;
	double GetSpeed() const;
    virtual void Print(std::ostream& out) const;

protected:
	virtual const AnimalData& GetAnimalData() const = 0;

private:
	std::string	m_name;
	std::string	m_species;
};


#endif // __ANIMAL_HPP__