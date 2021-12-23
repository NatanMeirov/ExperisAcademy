#ifndef SF_ISER_FW_OBJ_FACTORY_HPP
#define SF_ISER_FW_OBJ_FACTORY_HPP


#include <memory> // std::shared_ptr
#include <type_traits> // std::is_same
#include "iser_fw_obj.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of an Abstract Factory of ISerFwObj.
// Each derived CONCRETE class of ISerFwObj, MUST supply an implementation of a
// derived concrete Factory class that creates a DEFAULT instance of the concrete class (later it will be transformed with other params).
// Concept pr RetPtrT: RetPtrT must me std::shared_ptr<infra::ISerFwObj> or infra::ISerFwObj* type
template <typename RetPtrT>
class ISerFwObjFactory
{
    static_assert(std::is_same<RetPtrT, std::shared_ptr<infra::ISerFwObj>>::value
        || std::is_same<RetPtrT, infra::ISerFwObj*>::value, "RetPtrT must be std::shared_ptr<infra::ISerFwObj> or infra::ISerFwObj*");

public:
    virtual ~ISerFwObjFactory() = default;
    virtual RetPtrT Create() const = 0;
};

} // infra

} // ser_fw


#endif // SF_ISER_FW_OBJ_FACTORY_HPP
