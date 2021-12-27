#ifndef SF_ISER_FW_OBJ_HPP
#define SF_ISER_FW_OBJ_HPP


#include <string>
#include <memory> // std::shared_ptr
#include "iser_fw_types.hpp"


namespace ser_fw
{

namespace infra
{

// The root interface of all Serializeable/Deserializeable objects in the Serialization Framework.
// Each derived concrete class must implement: TypeName() - that returns string that should be similar to its class name, FromParamsMap() into self,
// and ToParamsMap() from self.
// All these are needed to be able to be serialized and deserialized without any dependency on a particular file/data format.
// [Note: The derived class should use insert() method to insert each key-value std::pair - in the implementation of TransformTo(),
// and should use at() method on each key to extract its matched value - in the implementation of TransformFrom()].
// Also, each derived concrete class must implement a factory class that inherit from ISerFwObjFactory
// and implement the Create() method - that would be used to create the object within the Serialization Framework, using the Reflector class.
// ParamsMap type: represents a {param:value} mapping (stored as {string:string} unordered_map) -
// while each key is a specific concrete param name, and the value is its matched concrete value.
class ISerFwObj
{
public:
    virtual ~ISerFwObj() = default;
    virtual std::string TypeName() const = 0;
    virtual Types::ParamsMap ToParamsMap() const = 0;
    virtual void FromParamsMap(const Types::ParamsMap& a_paramsMap) = 0;
};

} // infra

} // ser_fw


#endif // SF_ISER_FW_OBJ_HPP
