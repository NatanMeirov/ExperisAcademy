#ifndef SF_SER_FW_OBJ_FACTORY_HPP
#define SF_SER_FW_OBJ_FACTORY_HPP


#include <memory> // std::shared_ptr
#include <type_traits> // std::is_same
#include "ser_fw_obj.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of an Abstract Factory of SerFwObj.
// Each derived CONCRETE class of SerFwObj, MUST supply an implementation of a
// derived concrete Factory class that creates a DEFAULT instance of the concrete class (later it will be transformed with other params).
// Concept pr RetPtrT: RetPtrT must me std::shared_ptr<infra::SerFwObj> or infra::SerFwObj* type
template <typename RetPtrT>
class SerFwObjFactory
{
    static_assert(std::is_same<RetPtrT, std::shared_ptr<infra::SerFwObj>>::value
        || std::is_same<RetPtrT, infra::SerFwObj*>::value, "RetPtrT must be std::shared_ptr<infra::SerFwObj> or infra::SerFwObj*");

public:
    virtual ~SerFwObjFactory() = default;
    virtual RetPtrT Create() const = 0;
};

} // infra

} // ser_fw


#endif // SF_SER_FW_OBJ_FACTORY_HPP
