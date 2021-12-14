#ifndef NM_LOCATION_HPP
#define NM_LOCATION_HPP


namespace smartbuilding
{

class Location
{
public:
    using FloorNumber = unsigned int;
    using RoomNumber = unsigned int;

    Location() = default;
    Location(FloorNumber a_floor, RoomNumber a_room): m_floor(a_floor), m_room(a_room) {}
    Location(const Location& a_other) = default;
    Location& operator=(const Location& a_other) = default;
    ~Location() = default;

    FloorNumber Floor() const { return m_floor; }
    RoomNumber Room() const { return m_room; }

private:
    FloorNumber m_floor;
    RoomNumber m_room;
};

} // smartbuilding


#endif // NM_LOCATION_HPP