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
// (to be used in a C++ STL container), and must implement: TypeName() as string (should be similar to its class name), TransformFrom() - from ParamsMap type
// and TransformTo() - to ParamsMap type, to be able to be serialized/deserialized without any dependency on a concrete file/data format.
// [The derived class should use insert() method to insert each key-value std::pair
// in the implementation of TransformTo(), and should use at() method on each key, to extract its matched value in the implementation of TransformFrom()].
// Also, each derived concrete class must implement a factory class that inherit from SerFwObjFactory
// and implement Create method - that should be used to create the object within the Serialization Framework, using the Reflector class.
// ParamsMap type: represents a {param:value} mapping (stored as {string:string} unordered_map) -
// while each key is a specific concrete param name, and the value is its matched concrete param's value.
class SerFwObj : public infra::ITransformable<Types::ParamsMap>
{
public:
    virtual ~SerFwObj() = default;
    virtual std::string TypeName() const = 0;
};

} // infra

} // ser_fw


#endif // SF_SER_FW_OBJ_HPP
