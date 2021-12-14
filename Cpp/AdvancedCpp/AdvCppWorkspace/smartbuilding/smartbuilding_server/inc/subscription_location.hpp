#ifndef NM_SUBSCRIPTION_LOCATION_HPP
#define NM_SUBSCRIPTION_LOCATION_HPP


#include <vector> // std::vector


namespace smartbuilding
{

// A subscription location indicator, to specify the location that a given device want to listen to
// Note: if a boolean flag is true - the related vector would not be checked
class SubscriptionLocation
{
public:
    SubscriptionLocation(bool a_isAllFloors, const std::vector<unsigned int>& a_specifiedFloors, bool a_isAllRooms, const std::vector<unsigned int>& a_specifiedRooms)
    : m_specifiedFloors(a_specifiedFloors)
    , m_specifiedRooms(a_specifiedRooms)
    , m_isAllFloors(a_isAllFloors)
    , m_isAllRooms(a_isAllRooms)
    {
    }
    SubscriptionLocation(const SubscriptionLocation& a_other) = default;
    SubscriptionLocation& operator=(const SubscriptionLocation& a_other) = default;
    ~SubscriptionLocation() = default;

    bool IsAllFloors() const { return m_isAllFloors; }
    bool IsAllRooms() const { return m_isAllRooms; }
    const std::vector<unsigned int>& SpecifiedFloors() const { return m_specifiedFloors; }
    const std::vector<unsigned int>& SpecifiedRooms() const { return m_specifiedRooms; }

private:
    std::vector<unsigned int> m_specifiedFloors;
    std::vector<unsigned int> m_specifiedRooms;
    bool m_isAllFloors;
    bool m_isAllRooms;
};

} // smartbuilding


#endif // NM_SUBSCRIPTION_LOCATION_HPP