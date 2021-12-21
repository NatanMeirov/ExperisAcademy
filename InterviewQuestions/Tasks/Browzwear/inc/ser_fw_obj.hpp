#ifndef SF_SER_FW_OBJ_HPP
#define SF_SER_FW_OBJ_HPP


#include <string>
#include <memory> // std::shared_ptr
#include "ser_fw_types.hpp"
#include "itransformable.hpp"
#include "iserialization_formatter.hpp"
#include "ideserialization_formatter.hpp"


namespace ser_fw
{

namespace infra
{

// The base class of all Serializeable/Deserializeable objects of the Serialization Framework.
// Each CONCRETE class must be default-constructable, copy-constructable and copy-assignable
// (to be able to be used in a container), and must implement TransformFrom() - ParamsMap and TransformTo() - ParamsMap,
// to be able to be serialized / deserialized without any dependency in a concrete datafile format.
// ParamsMap type: represents a {param:value} mapping (represented as {string:string} map) -
// while each key is a specific concrete param name, and the value is its matched concrete param's value.
class SerFwObj : public infra::ITransformable<Types::ParamsMap>
{
public:
    using FormattedString = std::string;

public:
    virtual ~SerFwObj() = default;
    FormattedString Serialize(std::shared_ptr<ISerializationFormatter> a_formatter);
    void Deserialize(std::shared_ptr<IDeserializationFormatter> a_formatter, const SerFwObj::FormattedString& a_formattedStr);
};

} // infra

} // ser_fw


#endif // SF_SER_FW_OBJ_HPP
