#ifndef SF_ISERIALIZATION_FORMATTER_HPP
#define SF_ISERIALIZATION_FORMATTER_HPP


#include <string>
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface for all Serialization Formatters.
// A concrete Serialization Formatter should handle an encoding process
// of a Types::TypedParamsMap, to its unique format representation.
// Note: A concrete Serialization Formatters must use the type info and contain it in the formatted string.
class ISerializationFormatter
{
public:
    virtual ~ISerializationFormatter() = default;
    virtual std::string Serialize(const Types::TypedParamsMap& a_typedMappedParams) const = 0;
};

} // infra

} // ser_fw


#endif // SF_ISERIALIZATION_FORMATTER_HPP
