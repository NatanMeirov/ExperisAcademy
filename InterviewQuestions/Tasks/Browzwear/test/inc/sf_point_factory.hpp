#ifndef SF_POINT_FACTORY_HPP
#define SF_POINT_FACTORY_HPP


#include <memory> // std::shared_ptr, std::make_shared
#include "ser_fw_obj.hpp"
#include "ser_fw_obj_factory.hpp"
#include "sf_point.hpp"


namespace ser_fw_test
{

class PointFactory : public ser_fw::infra::SerFwObjFactory<std::shared_ptr<ser_fw::infra::SerFwObj>>
{
public:
    virtual ~PointFactory() = default;
    virtual std::shared_ptr<ser_fw::infra::SerFwObj> Create() const override { return std::make_shared<Point>(); }
};


class RawPointFactory : public ser_fw::infra::SerFwObjFactory<ser_fw::infra::SerFwObj*>
{
public:
    virtual ~RawPointFactory() = default;
    virtual ser_fw::infra::SerFwObj* Create() const override { return new Point(); }
};

} // ser_fw_test


#endif // SF_POINT_FACTORY_HPP
