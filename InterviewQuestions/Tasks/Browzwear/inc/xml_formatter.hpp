#ifndef SF_XML_FORMATTER_HPP
#define SF_XML_FORMATTER_HPP


#include <string>
#include "iserialization_formatter.hpp"
#include "ideserialization_formatter.hpp"


namespace ser_fw
{

// A concrete XML serialization/deserialization formatter class.
class XMLFormatter : public infra::ISerializationFormatter, public infra::IDeserializationFormatter
{
public:
    using XMLFormatString = std::string;

public:
    virtual ~XMLFormatter() = default;
    virtual std::string Encode(const Types::ParamsMap& a_mappedParams) const override { return ToXml(a_mappedParams); }
    virtual Types::ParamsMap Decode(const std::string& a_formattedStr) const override { return FromXml(a_formattedStr); }

private:
    XMLFormatString ToXml(const Types::ParamsMap& a_mappedParams) const;
    Types::ParamsMap FromXml(const XMLFormatString& a_xmlStr) const;
};

} // ser_fw


#endif // SF_XML_FORMATTER_HPP
