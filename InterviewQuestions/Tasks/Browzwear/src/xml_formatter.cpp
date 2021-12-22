#include "xml_formatter.hpp"


namespace ser_fw
{

XMLFormatter::XMLFormatString XMLFormatter::ToXml(const Types::TypedParamsMap& a_typedMappedParams) const
{
    // TODO: convert the mapped params into a string of xml format, using the type string as type indicator
}


Types::TypedParamsMap XMLFormatter::FromXml(const XMLFormatter::XMLFormatString& a_xmlStr) const
{
    // TODO: convert the xml string into a mapped params and a type string pair
}

} // ser_fw
