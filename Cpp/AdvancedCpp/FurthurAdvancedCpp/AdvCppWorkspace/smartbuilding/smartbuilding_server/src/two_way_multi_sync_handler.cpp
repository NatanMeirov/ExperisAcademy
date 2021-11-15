#include "two_way_multi_sync_handler.hpp"
#include "atomic_value.hpp"
#include "barrier.hpp"


advcpp::TwoWayMultiSyncHandler::TwoWayMultiSyncHandler()
: m_firstDirectionNotifications(0)
, m_secondDirectionSignals(1) // Dummy initialization
{
}


void advcpp::TwoWayMultiSyncHandler::Notify(size_t a_toNotify)
{
    m_firstDirectionNotifications.Add(a_toNotify);
}


size_t advcpp::TwoWayMultiSyncHandler::NotificationsCount() const
{
    return m_firstDirectionNotifications.Get();
}


void advcpp::TwoWayMultiSyncHandler::ResetAllNotifications()
{
    m_firstDirectionNotifications.Set(0);
}


void advcpp::TwoWayMultiSyncHandler::OneNotificationAccept()
{
    --m_firstDirectionNotifications;
}


void advcpp::TwoWayMultiSyncHandler::SignalBack()
{
    m_secondDirectionSignals.Wait();
}


bool advcpp::TwoWayMultiSyncHandler::WaitForAllSignalsBack()
{
    m_secondDirectionSignals.Wait();
    return true;
}


void advcpp::TwoWayMultiSyncHandler::SetWantedSignalsBack(size_t a_signalsToWaitFor)
{
    m_secondDirectionSignals.Reset(a_signalsToWaitFor + 1); // +1 for the notifier itself
}
