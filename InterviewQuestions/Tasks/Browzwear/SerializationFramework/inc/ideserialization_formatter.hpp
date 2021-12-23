#ifndef SF_IDESERIALIZATION_FORMATTER_HPP
#define SF_IDESERIALIZATION_FORMATTER_HPP


#include <string>
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface for all Deserialization Formatters.
// A concrete Deserialization Formatter should handle a decoding process
// of its unique format representation, to Types::TypedParamsMap.
// Note: A concrete Deserialization Formatters must extract the type info and specify it in the created Types::TypedParamsMap.
class IDeserializationFormatter
{
public:
    virtual ~IDeserializationFormatter() = default;
    virtual Types::TypedParamsMap Deserialize(const std::string& a_formattedStr) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IDESERIALIZATION_FORMATTER_HPP
