#ifndef NM_SERIALIZED_OBJECT_HPP
#define NM_SERIALIZED_OBJECT_HPP


#include <string> // std::string


namespace smartbuilding
{

// A serialized object representation object, that should be created after parsing the configurations file
struct SerializedObject
{
    std::string m_id;
    std::string m_type;
    unsigned int m_room;
    unsigned int m_floor;
    std::string m_logFileName;
    std::string m_configurations;
    std::string m_soName;
};

} // smartbuilding


#endif // NM_SERIALIZED_OBJECT_HPP
