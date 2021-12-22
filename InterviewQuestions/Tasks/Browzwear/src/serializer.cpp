#include "serializer.hpp"


namespace ser_fw
{

namespace infra
{

Serializer::FormattedString Serializer::Serialize(std::shared_ptr<ISerializationFormatter> a_formatter, const Types::TypedParamsMap& a_typedMappedParams) const
{
    return a_formatter->Encode(a_typedMappedParams);
}

} // infra

} // ser_fw
