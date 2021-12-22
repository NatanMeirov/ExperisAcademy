#include "json_formatter.hpp"


namespace ser_fw
{

JsonFormatter::JsonFormatString JsonFormatter::ToJson(const Types::TypedParamsMap& a_typedMappedParams) const
{
    // TODO: convert the mapped params into a string of json format, using the type string as type indicator
}


Types::TypedParamsMap JsonFormatter::FromJson(const JsonFormatter::JsonFormatString& a_jsonStr) const
{
    // TODO: convert the json string into a mapped params and a type string pair
}

} // ser_fw
