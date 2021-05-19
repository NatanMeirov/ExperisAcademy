#ifndef __MATCHINGSUBSTRINGINNAMECRITERIA_HPP__
#define __MATCHINGSUBSTRINGINNAMECRITERIA_HPP__


#include "FindItemCriteria.hpp"
#include <string> // std::string
#include "CollectableItem.hpp"


// To use this criteria: the CollectibleItem must be an ArtWork
struct MatchingSubStringInNameCriteria : public FindItemCriteria {
    MatchingSubStringInNameCriteria(const std::string& a_subStringToSearch);

    virtual bool IsPassingCriteria(const CollectableItem &a_item) override;

    // Members
    std::string m_subString;
};


#endif // __MATCHINGSUBSTRINGINNAMECRITERIA_HPP__