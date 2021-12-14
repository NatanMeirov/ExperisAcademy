#ifndef NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HPP
#define NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HPP


#include <memory> // std::shared_ptr


namespace advcpp
{

template <typename T>
class SafeRefCountingSharedPtrWrapper
{
public:
    explicit SafeRefCountingSharedPtrWrapper(std::shared_ptr<T> a_wrappedSharedPtr);
    ~SafeRefCountingSharedPtrWrapper() = default;

    std::shared_ptr<T>& GetInnerSharedPtr();

private:
    std::shared_ptr<T> m_wrappedSharedPtr; // Capture by value (copy) -> ++ref_count
};

} // advcpp


#include "inl/safe_ref_counting_shared_ptr_wrapper.hxx"


#endif // NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HPP