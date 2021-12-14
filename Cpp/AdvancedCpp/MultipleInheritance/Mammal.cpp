#include "Mammal.hpp"
#include "MammalData.hpp"


double Mammal::GetDurationOfPregnancy() const {
	return GetMammalData().m_durationOfPregnancy;
}


size_t Mammal::GetNumOfBornInLitter() const {
	return GetMammalData().m_numOfBornInLitter;
}


void Mammal::Print(std::ostream& a_os) const {
    this->Animal::Print(a_os);
	a_os << "Duration of Pregnancy: " << this->GetDurationOfPregnancy() << "\n";
	a_os << "Num Of Born In Litter: " << this->GetNumOfBornInLitter() << "\n";
}
