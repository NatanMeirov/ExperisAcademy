#ifndef NM_EVENT_LOCATION_HPP
#define NM_EVENT_LOCATION_HPP


namespace smartbuilding
{

class EventLocation
{
public:
    using BuildingNumber = unsigned int;
    using FloorNumber = unsigned int;
    using RoomNumber = unsigned int;

    EventLocation(BuildingNumber a_building, FloorNumber a_floor, RoomNumber a_room): m_building(a_building), m_floor(a_floor), m_room(a_room) {}
    EventLocation(const EventLocation& a_other) = default;
    EventLocation& operator=(const EventLocation& a_other) = default;
    ~EventLocation() = default;

    BuildingNumber Building() const { return m_building; }
    FloorNumber Floor() const { return m_floor; }
    RoomNumber Room() const { return m_room; }

private:
    BuildingNumber m_building;
    FloorNumber m_floor;
    RoomNumber m_room;
};

} // smartbuilding


#endif // NM_EVENT_LOCATION_HPP