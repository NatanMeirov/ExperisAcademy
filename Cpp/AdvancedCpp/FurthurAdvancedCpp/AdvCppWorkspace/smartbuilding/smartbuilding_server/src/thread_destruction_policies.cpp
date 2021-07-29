#include "thread_destruction_policies.hpp"
#include <cassert> // assert
#include "thread.hpp"
#include "atomic_value.hpp"


void advcpp::JoinPolicy::operator()(Thread<JoinPolicy>& a_thread)
{
    a_thread.ConditionalJoin();
}


void advcpp::DetachPolicy::operator()(Thread<DetachPolicy>& a_thread)
{
    a_thread.ConditionalDetach();
}


void advcpp::CancelPolicy::operator()(Thread<CancelPolicy>& a_thread)
{
    a_thread.ConditionalCancel(m_ensureCompleteCancelation);
}


void advcpp::AssertionPolicy::operator()(Thread<AssertionPolicy>& a_thread)
{
    assert(!a_thread.m_isAvailableThread.Check());
}
