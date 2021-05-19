#include "../Include/MatchingSubStringInNameCriteria.hpp"
#include <string> // std::string
#include "../Include/CollectableItem.hpp"
#include "../Include/ArtWork.hpp"


MatchingSubStringInNameCriteria::MatchingSubStringInNameCriteria(const std::string& a_subStringToSearch)
: m_subString(a_subStringToSearch) {
}


bool MatchingSubStringInNameCriteria::IsPassingCriteria(const CollectableItem &a_item) {
    // Must be casted first to an ArtWork - to get to its name
    return static_cast<const ArtWork&>(a_item).GetArtWorkName().find(this->m_subString) != std::string::npos;
}