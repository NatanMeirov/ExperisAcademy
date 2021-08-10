#ifndef NM_ICONFIG_READER_HPP
#define NM_ICONFIG_READER_HPP


#include <string> // std::string
#include <deque> // std::deque
#include "serialized_object.hpp"


namespace smartbuilding
{

// An interface of config file reader
class IConfigReader
{
public:
    using SerializedObjectCollection = std::deque<SerializedObject>;

    virtual ~IConfigReader() = default;
    virtual SerializedObjectCollection ReadConfig(const std::string& a_configFile) = 0;
};

} // smartbuilding


#endif // NM_ICONFIG_READER_HPP
