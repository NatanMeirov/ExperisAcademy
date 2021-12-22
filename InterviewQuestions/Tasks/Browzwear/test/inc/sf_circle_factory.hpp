#ifndef SF_CIRCLE_FACTORY_HPP
#define SF_CIRCLE_FACTORY_HPP


#include <memory> // std::shared_ptr, std::make_shared
#include "ser_fw_obj.hpp"
#include "ser_fw_obj_factory.hpp"
#include "sf_circle.hpp"


namespace ser_fw_test
{

class CircleFactory : public ser_fw::infra::SerFwObjFactory
{
public:
    virtual ~CircleFactory() = default;
    virtual std::shared_ptr<ser_fw::infra::SerFwObj> Create() const override { return std::make_shared<Circle>(); }
};

} // ser_fw_test


#endif // SF_CIRCLE_FACTORY_HPP
