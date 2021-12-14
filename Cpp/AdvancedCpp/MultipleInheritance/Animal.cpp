#include "Animal.hpp"
#include "AnimalData.hpp"
#include "Utils.hpp"


Animal::Animal(const std::string& a_name, const std::string& a_species)
: m_name(a_name)
, m_species(a_species){
}


std::string Animal::GetName() const {
	return this->m_name;
}


std::string Animal::GetSpecies() const {
	return this->m_species;
}


double Animal::GetLifeExpectancy() const {
	return this->GetAnimalData().m_lifeExpectancy;
}


std::vector<std::string> Animal::GetContinents() const {
	return GetAnimalData().m_continents;
}


std::vector<std::string> Animal::GetFood() const {
	return GetAnimalData().m_food;
}


double Animal::GetSpeed() const {
	return GetAnimalData().m_speed;
}


std::ostream& operator<<(std::ostream& a_os, const Animal& animal) {
	animal.Print(a_os);
	return a_os;
}


void Animal::Print(std::ostream& a_os) const {
	a_os << "Animal's Name: " << this->GetName() << "\n";
	a_os << "Animal's Species: " << this->GetSpecies() << "\n";
	a_os << "Animal's Life Expectency: " << this->GetLifeExpectancy() << "\n";
	a_os << "Animal's Continents: "; PrintCollection(this->GetContinents(), a_os); a_os << "\n";
	a_os << "Animal's Food: "; PrintCollection(this->GetFood(), a_os); a_os << "\n";
	a_os << "Animal's Speed: " << this->GetSpeed() << "\n";
}