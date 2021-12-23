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
    virtual std::string Serialize(const Types::TypedParamsMap& a_typedMappedParams) const override { return ToJson(a_typedMappedParams); }
    virtual Types::TypedParamsMap Deserialize(const std::string& a_formattedStr) const override { return FromJson(a_formattedStr); }

private:
    JsonFormatString ToJson(const Types::TypedParamsMap& a_typedMappedParams) const;
    Types::TypedParamsMap FromJson(const JsonFormatString& a_jsonStr) const;
};

} // ser_fw


#endif // SF_JSON_FORMATTER_HPP
