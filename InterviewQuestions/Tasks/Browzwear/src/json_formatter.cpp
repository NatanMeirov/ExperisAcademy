#include "json_formatter.hpp"


namespace ser_fw
{

JsonFormatter::JsonFormatString JsonFormatter::ToJson(const Types::ParamsMap& a_mappedParams) const
{
    // TODO: convert the mapped params into a string of json format
}


Types::ParamsMap JsonFormatter::FromJson(const JsonFormatter::JsonFormatString& a_jsonStr) const
{
    // TODO: convert the json string into a mapped params
}

} // ser_fw
