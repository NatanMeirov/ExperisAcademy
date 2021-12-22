#ifndef SF_EXAMPLE_FORMATTER_HPP
#define SF_EXAMPLE_FORMATTER_HPP


#include "iserialization_formatter.hpp"
#include "ideserialization_formatter.hpp"


namespace ser_fw_test
{

// A test class of a formatter that formats a CSV line in form: {type:...,param|val...}, into a TypedParamsMap
class ExampleFormatter
    : public ser_fw::infra::ISerializationFormatter, public ser_fw::infra::IDeserializationFormatter
{
public:
    virtual std::string Serialize(const ser_fw::Types::TypedParamsMap &a_typedMappedParams) const override;
    virtual ser_fw::Types::TypedParamsMap Deserialize(const std::string &a_formattedStr) const override;
};

} // ser_fw_test


#endif // SF_EXAMPLE_FORMATTER_HPP
