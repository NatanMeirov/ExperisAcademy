#include "../Include/PublishYearInCorrectRangeCriteria.hpp"
#include "../Include/CollectableItem.hpp"
#include "../Include/ArtWork.hpp"


PublishYearInCorrectRangeCriteria::PublishYearInCorrectRangeCriteria(const unsigned int& a_minimumPublishYear)
: m_minimumPublishYear(a_minimumPublishYear) {
}


bool PublishYearInCorrectRangeCriteria::IsPassingCriteria(const CollectableItem& a_item) const {
    // Must be casted first to an ArtWork - to get to its publish year
    return static_cast<const ArtWork&>(a_item).GetPublishYear() >= this->m_minimumPublishYear;
}