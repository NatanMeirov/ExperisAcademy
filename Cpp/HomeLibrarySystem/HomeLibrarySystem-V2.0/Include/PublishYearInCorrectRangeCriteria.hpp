#ifndef __PUBLISHYEARINSORRECTRANGECRITERIA_HPP__
#define __PUBLISHYEARINSORRECTRANGECRITERIA_HPP__


#include "FindItemCriteria.hpp"
#include "CollectableItem.hpp"


// To use this criteria: the CollectibleItem must be an ArtWork
struct PublishYearInCorrectRangeCriteria : public FindItemCriteria {
    PublishYearInCorrectRangeCriteria(const unsigned int& a_minimumPublishYear);

    virtual bool IsPassingCriteria(const CollectableItem& a_item) override;

    // Members
    unsigned int m_minimumPublishYear;
};


#endif // __PUBLISHYEARINSORRECTRANGECRITERIA_HPP__