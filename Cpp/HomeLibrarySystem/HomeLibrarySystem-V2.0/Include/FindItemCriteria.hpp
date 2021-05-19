#ifndef __FINDITEMCRITERIA_HPP__
#define __FINDITEMCRITERIA_HPP__


#include "CollectableItem.hpp"


// A base class for find criteria of each CollectableItem, default definition of the boolean function - returns true
struct FindItemCriteria {
    virtual bool IsPassingCriteria(const CollectableItem& a_item);
};


#endif // __FINDITEMCRITERIA_HPP__
