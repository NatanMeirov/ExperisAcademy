#ifndef SF_CIRCLE_FACTORY_HPP
#define SF_CIRCLE_FACTORY_HPP


#include <memory> // std::shared_ptr, std::make_shared
#include "iser_fw_obj.hpp"
#include "iser_fw_obj_factory.hpp"
#include "sf_circle.hpp"


namespace ser_fw_test
{

class CircleFactory : public ser_fw::infra::ISerFwObjFactory<std::shared_ptr<ser_fw::infra::ISerFwObj>>
{
public:
    virtual ~CircleFactory() = default;
    virtual std::shared_ptr<ser_fw::infra::ISerFwObj> Create() const override { return std::make_shared<Circle>(); }
};


class RawCircleFactory : public ser_fw::infra::ISerFwObjFactory<ser_fw::infra::ISerFwObj*>
{
public:
    virtual ~RawCircleFactory() = default;
    virtual ser_fw::infra::ISerFwObj* Create() const override { return new Circle(); }
};

} // ser_fw_test


#endif // SF_CIRCLE_FACTORY_HPP
