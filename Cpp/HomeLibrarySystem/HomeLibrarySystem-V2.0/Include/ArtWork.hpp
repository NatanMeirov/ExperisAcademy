#ifndef __ARTWORK_HPP__
#define __ARTWORK_HPP__


#include <string> // std::string
#include "CollectableItem.hpp"


class ArtWork : public CollectableItem {
public:
    static const int ARTWORK_MIN_PUBLISHED_YEAR = 1900;

    ArtWork(const std::string& a_artWorkName, const std::string& a_publisherName, const unsigned int& a_publishYear, const std::string& a_artWorkType);
    ArtWork(const ArtWork& a_other);
    ArtWork& operator=(const ArtWork& a_other);
    ~ArtWork();

    // Const Methods
    const std::string& GetArtWorkName() const;
    const std::string& GetPublisherName() const;
    const unsigned int& GetPublishYear() const;

private:
    std::string m_artWorkName;
    std::string m_publisherName;
    unsigned int m_publishYear;
};


#endif // __ARTWORK_HPP__