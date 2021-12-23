#ifndef SF_POINT_FACTORY_HPP
#define SF_POINT_FACTORY_HPP


#include <memory> // std::shared_ptr, std::make_shared
#include "iser_fw_obj.hpp"
#include "iser_fw_obj_factory.hpp"
#include "sf_point.hpp"


namespace ser_fw_test
{

class PointFactory : public ser_fw::infra::ISerFwObjFactory<std::shared_ptr<ser_fw::infra::ISerFwObj>>
{
public:
    virtual ~PointFactory() = default;
    virtual std::shared_ptr<ser_fw::infra::ISerFwObj> Create() const override { return std::make_shared<Point>(); }
};


class RawPointFactory : public ser_fw::infra::ISerFwObjFactory<ser_fw::infra::ISerFwObj*>
{
public:
    virtual ~RawPointFactory() = default;
    virtual ser_fw::infra::ISerFwObj* Create() const override { return new Point(); }
};

} // ser_fw_test


#endif // SF_POINT_FACTORY_HPP
