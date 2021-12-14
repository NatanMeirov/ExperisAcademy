#include "Fish.hpp"
#include "FishData.hpp"


double Fish::GetLowestDepth() const {
	return GetFishData().m_lowestDepth;
}


void Fish::Print(std::ostream& a_os) const {
    this->Animal::Print(a_os);
	a_os << "Lowest Depth: " << this->GetLowestDepth() << "\n";
}
