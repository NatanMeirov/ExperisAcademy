#include "deserializer.hpp"


#include "serializer.hpp"


namespace ser_fw
{

namespace infra
{

Types::TypedParamsMap Deserializer::Deserialize(std::shared_ptr<IDeserializationFormatter> a_formatter, const FormattedString& a_formattedStr) const
{
    return a_formatter->Decode(a_formattedStr);
}

} // infra

} // ser_fw
