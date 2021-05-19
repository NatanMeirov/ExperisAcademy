#ifndef __COLLECTABLEITEM_HPP__
#define __COLLECTABLEITEM_HPP__


#include <string> // std::string


// A polymorphic base for all collectable items
class CollectableItem {
public:

    CollectableItem(const std::string& a_collectableItemType);
    CollectableItem(const std::string& a_collectableItemType, const unsigned int& a_timeAddedToCollection);
    CollectableItem(const CollectableItem& a_other);
    CollectableItem& operator=(const CollectableItem& a_other);
    ~CollectableItem();

    // Const Methods
    const std::string& GetCollectableItemType() const;
    const unsigned int& GetTimeAddedToCollection() const;

    // Methods
    void SetTimeAddedToCollection(const unsigned int& a_timeAddedToCollection);

private:
    std::string m_collectableItemType;
    unsigned int m_timeAddedToCollection; // Default value (not initialized): 0
};


#endif // __COLLECTABLEITEM_HPP__