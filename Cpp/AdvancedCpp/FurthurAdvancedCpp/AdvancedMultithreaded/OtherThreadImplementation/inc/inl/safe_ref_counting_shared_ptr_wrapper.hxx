#ifndef NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HXX
#define NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HXX


#include <memory> // std::shared_ptr


namespace advcpp
{

template <typename T>
SafeRefCountingSharedPtrWrapper<T>::SafeRefCountingSharedPtrWrapper(std::shared_ptr<T> a_wrappedSharedPtr)
: m_wrappedSharedPtr(a_wrappedSharedPtr)
{
}


template <typename T>
std::shared_ptr<T>& SafeRefCountingSharedPtrWrapper<T>::GetInnerSharedPtr()
{
    return m_wrappedSharedPtr;
}

} // advcpp



#endif // NM_SAFE_REF_COUNTING_SHARED_PTR_WRAPPER_HXX