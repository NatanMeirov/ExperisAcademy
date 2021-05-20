#include "../Include/CollectedYearInCorrectRangeCriteria.hpp"
#include "../Include/CollectableItem.hpp"


CollectedYearInCorrectRangeCriteria::CollectedYearInCorrectRangeCriteria(const unsigned int& a_minimumCollectedYear)
: m_minimumCollectedYear(a_minimumCollectedYear) {
}


bool CollectedYearInCorrectRangeCriteria::IsPassingCriteria(const CollectableItem& a_item) const {
    // Must be casted first to an ArtWork - to get to its publish year
    return a_item.GetTimeAddedToCollection() >= this->m_minimumCollectedYear;
}