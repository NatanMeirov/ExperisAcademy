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
// Each derived CONCRETE class must be default-constructable, copy-constructable and copy-assignable
// (used in a stl container), and must implement TransformFrom() - from ParamsMap type
// and TransformTo() - to ParamsMap type, to be able to be serialized/deserialized without any dependency in a concrete datafile format.
// [The derived class should use insert() method to insert each key-value std::pair
// in the implementation of TransformTo(), and should use at() method on each key, to extract its matched value in the implementation of TransformFrom()].
// ParamsMap type: represents a {param:value} mapping (stored as {string:string} unordered_map) -
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
