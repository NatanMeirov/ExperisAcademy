#include "../Include/ArtWork.hpp"
#include <string> // std::string
#include "../Include/CollectableItem.hpp"


ArtWork::ArtWork(const std::string& a_artWorkName, const std::string& a_publisherName, const unsigned int& a_publishYear, const std::string& a_artWorkType)
: CollectableItem(a_artWorkType)
, m_artWorkName(a_artWorkName)
, m_publisherName(a_publisherName)
, m_publishYear(a_publishYear) {
}


ArtWork::ArtWork(const ArtWork& a_other)
: CollectableItem(a_other) // Refers to a_other as a CollectableItem to be used in the copy c'tor to initialize the base class' part of "this"
, m_artWorkName(a_other.m_artWorkName)
, m_publisherName(a_other.m_publisherName)
, m_publishYear(a_other.m_publishYear) {
}


ArtWork& ArtWork::operator=(const ArtWork& a_other) {
    this->CollectableItem::operator=(a_other); // Refers to a_other as a CollectableItem to be used in the copy-assignment operator to assign the base class' part of "this"
    this->m_artWorkName = a_other.m_artWorkName;
    this->m_publisherName = a_other.m_publisherName;
    this->m_publishYear = a_other.m_publishYear;

    return *this;
}


ArtWork::~ArtWork() {
}


const std::string& ArtWork::GetArtWorkName() const {
    return this->m_artWorkName;
}


const std::string& ArtWork::GetPublisherName() const {
    return this->m_publisherName;
}


const unsigned int& ArtWork::GetPublishYear() const {
    return this->m_publishYear;
}