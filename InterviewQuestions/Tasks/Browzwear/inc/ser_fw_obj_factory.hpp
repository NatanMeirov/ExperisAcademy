#ifndef SF_SER_FW_OBJ_FACTORY_HPP
#define SF_SER_FW_OBJ_FACTORY_HPP


#include <memory> // std::shared_ptr
#include "ser_fw_obj.hpp"


namespace ser_fw
{

namespace infra
{

// An interface of an Abstract Factory of SerFwObj.
// Each derived CONCRETE class of SerFwObj, MUST supply an implementation of a
// derived concrete Factory class that creates a DEFAULT instance of the concrete class (later it will be transformed with other params).
class SerFwObjFactory
{
public:
    virtual ~SerFwObjFactory() = default;
    virtual std::shared_ptr<SerFwObj> Create() const = 0;
};

} // infra

} // ser_fw


#endif // SF_SER_FW_OBJ_FACTORY_HPP
