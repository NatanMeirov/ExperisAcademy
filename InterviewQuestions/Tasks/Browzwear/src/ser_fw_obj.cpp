#include "ser_fw_obj.hpp"
#include <iserialization_formatter.hpp>
#include <ideserialization_formatter.hpp>


namespace ser_fw
{

namespace infra
{

SerFwObj::FormattedString SerFwObj::Serialize(std::shared_ptr<ISerializationFormatter> a_formatter)
{
    return a_formatter->Encode(TransformTo());
}


void SerFwObj::Deserialize(std::shared_ptr<IDeserializationFormatter> a_formatter, const SerFwObj::FormattedString& a_formattedStr)
{
    TransformFrom(a_formatter->Decode(a_formattedStr));
}

} // infra

} // ser_fw
