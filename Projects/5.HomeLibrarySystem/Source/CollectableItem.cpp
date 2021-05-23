#include "../Include/CollectableItem.hpp"
#include <string> // std::string


CollectableItem::CollectableItem(const std::string& a_collectableItemType)
: m_collectableItemType(a_collectableItemType)
, m_timeAddedToCollection(0) {
}


CollectableItem::CollectableItem(const std::string& a_collectableItemType, const unsigned int& a_timeAddedToCollection)
: m_collectableItemType(a_collectableItemType)
, m_timeAddedToCollection(a_timeAddedToCollection){
}


CollectableItem::CollectableItem(const CollectableItem& a_other)
: m_collectableItemType(a_other.m_collectableItemType)
, m_timeAddedToCollection(a_other.m_timeAddedToCollection){
}


CollectableItem& CollectableItem::operator=(const CollectableItem& a_other) {
    this->m_collectableItemType = a_other.m_collectableItemType;
    this->m_timeAddedToCollection = a_other.m_timeAddedToCollection;

    return *this;
}


CollectableItem::~CollectableItem(){
}


const std::string& CollectableItem::GetCollectableItemType() const {
    return this->m_collectableItemType;
}


const unsigned int& CollectableItem::GetTimeAddedToCollection() const {
    return this->m_timeAddedToCollection;
}


void CollectableItem::SetTimeAddedToCollection(const unsigned int& a_timeAddedToCollection) {
    this->m_timeAddedToCollection = a_timeAddedToCollection;
}