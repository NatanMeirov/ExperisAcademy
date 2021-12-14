#ifndef NM_CALLABLE_FUNCTIONS_ADAPTERS
#define NM_CALLABLE_FUNCTIONS_ADAPTERS


#include "icallable.hpp"


namespace advcpp
{

/* An adapter classes for pre-constructed Functors, or regular C/C++ global functions */

/* VV - void(void) */
template<typename Func>
class NonICallableToICallableAdapterVV : public ICallable
{
public:
    NonICallableToICallableAdapterVV(const Func& a_func) : m_func(a_func) {}
    virtual ~NonICallableToICallableAdapterVV() = default;

    virtual void operator()() override { m_func(); }

private:
    Func m_func;
};


/* RV - RetT(void) */
// Concept of RetT: RetT MUST NOT be (void), and must be default-constructable and copy-constructable
// Note: ReturnValue() should be called AFTER the execution of the adapter, to supply the correct answer
template<typename Func, typename RetT>
class NonICallableToICallableAdapterRV : public ICallable
{
public:
    NonICallableToICallableAdapterRV(const Func& a_func) : m_func(a_func), m_retVal() {}
    virtual ~NonICallableToICallableAdapterRV() = default;

    virtual void operator()() override { m_retVal = m_func(); }
    RetT ReturnValue() { return m_retVal; }

private:
    Func m_func;
    RetT m_retVal;
};


/* VA - void(Arg) */
// Concept of Arg: Arg must be copy-constructable
template<typename Func, typename Arg>
class NonICallableToICallableAdapterVA : public ICallable
{
public:
    NonICallableToICallableAdapterVA(const Func& a_func, const Arg& a_arg) : m_func(a_func), m_arg(a_arg) {}
    virtual ~NonICallableToICallableAdapterVA() = default;

    virtual void operator()() override { m_func(m_arg); }

private:
    Func m_func;
    Arg m_arg;
};


/* RA - RetT(Arg) */
// Concept of RetT: RetT MUST NOT be (void), and must be default-constructable and copy-constructable
// Concept of Arg: Arg must be copy-constructable
// Note: ReturnValue() should be called AFTER the execution of the adapter, to supply the correct answer
template<typename Func, typename RetT, typename Arg>
class NonICallableToICallableAdapterRA : public ICallable
{
public:
    NonICallableToICallableAdapterRA(const Func& a_func, const Arg& a_arg) : m_func(a_func), m_retVal(), m_arg(a_arg) {}
    virtual ~NonICallableToICallableAdapterRA() = default;

    virtual void operator()() override { m_retVal = m_func(m_arg); }
    RetT ReturnValue() { return m_retVal; }

private:
    Func m_func;
    RetT m_retVal;
    Arg m_arg;
};


/* RAA - RetT(ArgA, ArgB) */
// Concept of RetT: RetT MUST NOT be (void), and must be default-constructable and copy-constructable
// Concept of ArgA and ArgB: Args must be copy-constructable
// Note: ReturnValue() should be called AFTER the execution of the adapter, to supply the correct answer
template<typename Func, typename RetT, typename ArgA, typename ArgB>
class NonICallableToICallableAdapterRAA : public ICallable
{
public:
    NonICallableToICallableAdapterRAA(const Func& a_func, const ArgA& a_argA, const ArgB& a_argB) : m_func(a_func), m_retVal(), m_argA(a_argA), m_argB(a_argB) {}
    virtual ~NonICallableToICallableAdapterRAA() = default;

    virtual void operator()() override { m_retVal = m_func(m_argA, m_argB); }
    RetT ReturnValue() { return m_retVal; }

private:
    Func m_func;
    RetT m_retVal;
    ArgA m_argA;
    ArgB m_argB;
};


/* RAAA - RetT(ArgA, ArgB, ArgC) */
// Concept of RetT: RetT MUST NOT be (void), and must be default-constructable and copy-constructable
// Concept of ArgA, ArgB and ArgC: Args must be copy-constructable
// Note: ReturnValue() should be called AFTER the execution of the adapter, to supply the correct answer
template<typename Func, typename RetT, typename ArgA, typename ArgB, typename ArgC>
class NonICallableToICallableAdapterRAAA : public ICallable
{
public:
    NonICallableToICallableAdapterRAAA(const Func& a_func, const ArgA& a_argA, const ArgB& a_argB, const ArgC& a_argC) : m_func(a_func), m_retVal(), m_argA(a_argA), m_argB(a_argB), m_argC(a_argC) {}
    virtual ~NonICallableToICallableAdapterRAAA() = default;

    virtual void operator()() override { m_retVal = m_func(m_argA, m_argB, m_argC); }
    RetT ReturnValue() { return m_retVal; }

private:
    Func m_func;
    RetT m_retVal;
    ArgA m_argA;
    ArgB m_argB;
    ArgC m_argC;
};


/* MFVV - Member Function: void(void) */
// Concept of ObjType: ObjType should be an object type, and should be a REFERENCE to the object itself
// Concept of MemFunc: MemFunc should be a pointer to a function ObjType::MemberFunctionName to be called (MUST bw a member function of ObjType obj)
template <typename ObjType, typename MemFunc>
class NonICallableToICallableAdapterMFVV : public ICallable
{
public:
    NonICallableToICallableAdapterMFVV(ObjType& a_obj, MemFunc* a_memFuncPtr) : m_obj(a_obj), m_memFuncPtr(a_memFuncPtr) {}
    virtual ~NonICallableToICallableAdapterMFVV() = default;

    virtual void operator()() override { (m_obj.*m_memFuncPtr)(); }

private:
    ObjType& m_obj;
    MemFunc* m_memFuncPtr;
};


/* MFRA - Member Function: RetT(Arg) */
// Concept of ObjType: ObjType should be an object type, and should be a REFERENCE to the object itself
// Concept of MemFunc: MemFunc should be a pointer to a function ObjType::MemberFunctionName to be called (MUST bw a member function of ObjType obj)
// Concept of RetT: RetT MUST NOT be (void), and must be default-constructable and copy-constructable
// Concept of ArgA, ArgB and ArgC: Args must be copy-constructable
// Note: ReturnValue() should be called AFTER the execution of the adapter, to supply the correct answer
template <typename ObjType, typename MemFunc, typename RetT, typename Arg>
class NonICallableToICallableAdapterMFRA : public ICallable
{
public:
    NonICallableToICallableAdapterMFRA(ObjType& a_obj, MemFunc* a_memFuncPtr, Arg a_arg) : m_obj(a_obj), m_memFuncPtr(a_memFuncPtr), m_retVal(), m_arg(a_arg) {}
    virtual ~NonICallableToICallableAdapterMFRA() = default;

    virtual void operator()() override { m_retVal = (m_obj.*m_memFuncPtr)(m_arg); }
    RetT ReturnValue() { return m_retVal; }

private:
    ObjType& m_obj;
    MemFunc* m_memFuncPtr;
    RetT m_retVal;
    Arg m_arg;
};

} // advcpp


#endif // NM_CALLABLE_FUNCTIONS_ADAPTERS