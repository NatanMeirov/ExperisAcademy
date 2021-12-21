#ifndef SF_SER_FR_TYPES_HPP
#define SF_SER_FR_TYPES_HPP


#include <string>
#include <vector>
#include <unordered_map>


namespace ser_fw
{

// A struct that holds the information about the commonly used types of the Serialization Framework.
struct Types
{
    // ParamsMap type: represents a param-value map (each key is a specific concrete param name, and its matched value)
    using ParamsMap = std::unordered_map<std::string, std::string>;

    // ParsedLinesCollection type: represents a collection of strings (in a specific format), to iterate over
    using ParsedLinesCollection = std::vector<std::string>;
};

} // ser_fw


#endif // SF_SER_FR_TYPES_HPP
