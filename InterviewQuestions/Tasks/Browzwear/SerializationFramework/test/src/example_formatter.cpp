#include "example_formatter.hpp"
#include <cstddef> // size_t
#include <string>
#include <utility> // std::move
#include <algorithm> // std::for_each


namespace ser_fw_test
{

std::string ExampleFormatter::Serialize(const ser_fw::Types::TypedParamsMap& a_typedMappedParams) const
{
    std::string formattedStr(a_typedMappedParams.first);
    formattedStr += "|";

    ser_fw::Types::ParamsMap map = a_typedMappedParams.second;
    std::for_each(map.begin(), map.end(),
    [&](const ser_fw::Types::ParamsMap::value_type& a_mapEntry)
    {
        formattedStr += a_mapEntry.first + "," + a_mapEntry.second + "|";
    });

    return formattedStr;
}


ser_fw::Types::TypedParamsMap ExampleFormatter::Deserialize(const std::string& a_formattedStr) const
{
    ser_fw::Types::TypedParamsMap typedParamsMap;
    size_t sepIndex = a_formattedStr.find("|");
    typedParamsMap.first = a_formattedStr.substr(0, sepIndex); // type name

    ser_fw::Types::ParamsMap map;
    size_t curSubStrStartIndex = sepIndex + 1;
    while((sepIndex = a_formattedStr.find("|", curSubStrStartIndex)) != std::string::npos)
    {
        std::string paramValuePairStr = a_formattedStr.substr(curSubStrStartIndex, (sepIndex - curSubStrStartIndex)); // a string in form: "param,value"
        size_t innerSepIndex = paramValuePairStr.find(",");
        std::string param = paramValuePairStr.substr(0, innerSepIndex);
        std::string value = paramValuePairStr.substr(innerSepIndex + 1); // innerSepIndex + 1 => "jumping" over the comma

        map.insert({param, value});

        curSubStrStartIndex = sepIndex + 1;
    }

    typedParamsMap.second = std::move(map);
    return typedParamsMap;
}

} // ser_fw_test
