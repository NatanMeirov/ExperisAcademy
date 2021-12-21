#ifndef SF_JSON_FORMATTER_HPP
#define SF_JSON_FORMATTER_HPP


#include <string>
#include "iserialization_formatter.hpp"
#include "ideserialization_formatter.hpp"


namespace ser_fw
{

// A concrete JSON serialization/deserialization formatter class.
class JsonFormatter : public infra::ISerializationFormatter, public infra::IDeserializationFormatter
{
public:
    using JsonFormatString = std::string;

public:
    virtual ~JsonFormatter() = default;
    virtual std::string Encode(const Types::ParamsMap& a_mappedParams) const override { return ToJson(a_mappedParams); }
    virtual Types::ParamsMap Decode(const std::string& a_formattedStr) const override { return FromJson(a_formattedStr); }

private:
    JsonFormatString ToJson(const Types::ParamsMap& a_mappedParams) const;
    Types::ParamsMap FromJson(const JsonFormatString& a_jsonStr) const;
};

} // ser_fw


#endif // SF_JSON_FORMATTER_HPP
