#include "../Include/FindItemCriteria.hpp"
#include "../Include/CollectableItem.hpp"


// Default definition of the boolean function - returns true
bool FindItemCriteria::IsPassingCriteria(const CollectableItem& a_item) {
    return true;
}