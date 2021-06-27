#include "../inc/OperatorTask.hpp"



nm::cdr::OperatorTask::OperatorTask(const std::string& a_mccmnc, const Cdr& a_cdrToAddToTable, std::unordered_map<std::string, OperatorInfoObj>& a_tableToUpdate)
: m_mccmnc(a_mccmnc)
, m_cdrToAddToTable(a_cdrToAddToTable)
, m_tableToUpdate(a_tableToUpdate){
}


void nm::cdr::OperatorTask::Execute() {
    
}