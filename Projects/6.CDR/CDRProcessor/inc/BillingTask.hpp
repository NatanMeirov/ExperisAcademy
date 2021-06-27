#ifndef __NM_CDR_BILLINGTASK_HPP__
#define __NM_CDR_BILLINGTASK_HPP__


#include <unordered_map>
#include "../../Infrastructure/inc/BillingInfoObj.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class BillingTask : public ICommand {
public:
    BillingTask(const uint64_t a_imsi, const Cdr& a_cdrToAddToTable, std::unordered_map<uint64_t, BillingInfoObj>& a_tableToUpdate);
    virtual ~BillingTask() = default;

    virtual void Execute() override;

private:
    void UpdateBillingInfoObjectAccordingCdr(BillingInfoObj& a_billingInfoObj) const;

    uint64_t m_imsi;
    Cdr m_cdrToAddToTable;
    std::unordered_map<uint64_t, BillingInfoObj>& m_tableToUpdate;
};

} // cdr

} // nm


#endif // __NM_CDR_BILLINGTASK_HPP__