#ifndef NM_TWO_WAY_MULTI_SYNC_HANDLER_HPP
#define NM_TWO_WAY_MULTI_SYNC_HANDLER_HPP


#include <cstddef> // size_t
#include "atomic_value.hpp"
#include "barrier.hpp"


namespace advcpp
{

// NOTE: before using back signals functionality, make sure to use SetWantedSignalsBack method - to provide the correct wanted signals!
class TwoWayMultiSyncHandler
{
public:
    TwoWayMultiSyncHandler();
    TwoWayMultiSyncHandler(const TwoWayMultiSyncHandler& a_other) = delete;
    TwoWayMultiSyncHandler& operator=(const TwoWayMultiSyncHandler& a_other) = delete;
    ~TwoWayMultiSyncHandler() = default;

    // First direction:
    void Notify(size_t a_toNotify);
    size_t NotificationsCount() const;
    void ResetNotifications();

    // Second direction:
    void AcceptNotification();
    void SignalBack();
    bool WaitForAllSignalsBack(); // Blocking wait, without polling
    void SetWantedSignalsBack(size_t a_signalsToWaitFor);

private:
    AtomicValue<size_t> m_firstDirectionNotifications;
    Barrier m_secondDirectionSignals;
};

} // advcpp


#endif // NM_TWO_WAY_MULTI_SYNC_HANDLER_HPP