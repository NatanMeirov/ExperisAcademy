#ifndef __COLLECTEDYEARINCORRECTRANGECRITERIA_HPP__
#define __COLLECTEDYEARINCORRECTRANGECRITERIA_HPP__


#include "FindItemCriteria.hpp"
#include "CollectableItem.hpp"


struct CollectedYearInCorrectRangeCriteria : public FindItemCriteria {
    CollectedYearInCorrectRangeCriteria(const unsigned int& a_minimumCollectedYear);

    virtual bool IsPassingCriteria(const CollectableItem& a_item) override;

    // Members
    unsigned int m_minimumCollectedYear;
};


#endif // __COLLECTEDYEARINCORRECTRANGECRITERIA_HPP__