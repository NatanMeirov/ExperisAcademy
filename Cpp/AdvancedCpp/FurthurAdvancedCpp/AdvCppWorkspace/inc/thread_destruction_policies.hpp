#ifndef NM_DESTRUCTION_POLICIES_HPP
#define NM_DESTRUCTION_POLICIES_HPP


#include <cassert> // assert
#include "thread.hpp"


namespace advcpp
{

class JoinPolicy
{
public:
    void operator()(Thread<JoinPolicy>& a_thread);
};


class DetachPolicy
{
public:
    void operator()(Thread<DetachPolicy>& a_thread);
};


class CancelPolicy
{
public:
    CancelPolicy(bool a_ensureCompleteCancelation = false) : m_ensureCompleteCancelation(a_ensureCompleteCancelation) {}
    void operator()(Thread<CancelPolicy>& a_thread);

private:
    bool m_ensureCompleteCancelation;
};


class AssertionPolicy
{
public:
    void operator()(Thread<AssertionPolicy>& a_thread);
};

} // advcpp


#endif // NM_DESTRUCTION_POLICIES_HPP