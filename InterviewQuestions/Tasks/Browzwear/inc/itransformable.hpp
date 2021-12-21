
#ifndef SF_ITRANSFORMABLE_HPP
#define SF_ITRANSFORMABLE_HPP


namespace ser_fw
{

namespace infra
{

// A generic interface of a transferable T object (from / to) concrete object.
// From: used to initialize the concrete object from T object.
// To: used to create a T object from the concrete object.
// Concept of T: T MUST be default-constructable, copy-constructable and copy-assignable.
template <typename T>
class ITransformable
{
public:
    virtual ~ITransformable() = default;
    virtual T TransformTo() const = 0;
    virtual void TransformFrom(const T& a_type) = 0;
};

} // infra

} // ser_fw


#endif // SF_ITRANSFORMABLE_HPP
