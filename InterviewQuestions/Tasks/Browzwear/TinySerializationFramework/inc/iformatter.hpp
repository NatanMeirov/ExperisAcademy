#ifndef SF_IFORMATTER_HPP
#define SF_IFORMATTER_HPP


#include <string>
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface for all Formatters (Serialization and Deserialization).
// A concrete Formatter should handle an encoding and decoding process:
// from its unique format to Types::TypeToParamsMapPair, and vice versa.
class IFormatter
{
public:
    virtual ~IFormatter() = default;
    virtual std::string Serialize(const Types::TypeToParamsMapPair& a_typedMappedParams) const = 0;
    virtual Types::TypeToParamsMapPair Deserialize(const std::string& a_formattedStr) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IFORMATTER_HPP
