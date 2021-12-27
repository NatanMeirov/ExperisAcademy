#ifndef SF_EXAMPLE_FORMATTER_HPP
#define SF_EXAMPLE_FORMATTER_HPP


#include "iformatter.hpp"


namespace ser_fw_test
{

// A test class of a formatter that formats a line of a form: {type|param1,val1|param2,val2|...}, into a TypeToParamsMapPair, and vice versa
class ExampleFormatter : public ser_fw::infra::IFormatter
{
public:
    virtual std::string Serialize(const ser_fw::Types::TypeToParamsMapPair& a_typedMappedParams) const override;
    virtual ser_fw::Types::TypeToParamsMapPair Deserialize(const std::string& a_formattedStr) const override;
};

} // ser_fw_test


#endif // SF_EXAMPLE_FORMATTER_HPP
