#ifndef SF_SER_FR_TYPES_HPP
#define SF_SER_FR_TYPES_HPP


#include <string>
#include <vector>
#include <utility> // std::pair
#include <unordered_map>


namespace ser_fw
{

// A struct that holds the information about the commonly used types of the Serialization Framework.
struct Types
{
    // ParamsMap type: represents a {param:value} mapping (stored as {string:string} unordered_map) -
    // while each key is a specific concrete param name, and the value is its matched concrete param's value.
    using ParamsMap = std::unordered_map<std::string, std::string>;

    // TypedParamsMap type: represents a {string:value} pair - of a type info (as string), and a matched ParamsMap of a concrete object from that type.
    using TypedParamsMap = std::pair<std::string, ParamsMap>;

    // ParsedLinesCollection type: represents a collection of strings (in a specific format), to iterate over
    using ParsedLinesCollection = std::vector<std::string>;
};

} // ser_fw


#endif // SF_SER_FR_TYPES_HPP
