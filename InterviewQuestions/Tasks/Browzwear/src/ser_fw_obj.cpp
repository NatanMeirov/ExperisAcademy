#include "ser_fw_obj.hpp"
#include <iserialization_formatter.hpp>
#include <ideserialization_formatter.hpp>


namespace ser_fw
{

namespace infra
{

SerFwObj::FormattedString SerFwObj::Serialize(std::shared_ptr<ISerializationFormatter> a_formatter)
{
    Types::ParamsMap map = TransformTo();
    map.insert({"type", TypeName()});
    return a_formatter->Encode(map);
}


void SerFwObj::Deserialize(std::shared_ptr<IDeserializationFormatter> a_formatter, const SerFwObj::FormattedString& a_formattedStr)
{
    Types::ParamsMap map = a_formatter->Decode(a_formattedStr);
    map.erase("type");
    TransformFrom(map);
}

} // infra

} // ser_fw
