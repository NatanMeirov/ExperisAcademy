#include "../inc/BillingTask.hpp"


nm::cdr::BillingTask::BillingTask(const uint64_t a_imsi, const Cdr& a_cdrToAddToTable, std::unordered_map<uint64_t, BillingInfoObj>& a_tableToUpdate)
: m_imsi(a_imsi)
, m_cdrToAddToTable(a_cdrToAddToTable)
, m_tableToUpdate(a_tableToUpdate){
}


void nm::cdr::BillingTask::Execute() {
    
}