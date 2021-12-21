#ifndef SF_IDESERIALIZATION_FORMATTER_HPP
#define SF_IDESERIALIZATION_FORMATTER_HPP


#include <string>
#include "ser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// An interface for all Deserialization Formatters.
// A concrete Deserialization Formatter should handle a decoding process
// of its unique format representation, to Types::ParamsMap.
class IDeserializationFormatter
{
public:
    virtual ~IDeserializationFormatter() = default;
    virtual Types::ParamsMap Decode(const std::string& a_formattedStr) const = 0;
};

} // infra

} // ser_fw


#endif // SF_IDESERIALIZATION_FORMATTER_HPP
