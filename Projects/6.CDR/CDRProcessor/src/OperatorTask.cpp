#include "../inc/OperatorTask.hpp"



nm::cdr::OperatorTask::OperatorTask(const std::string& a_mccmnc, const Cdr& a_cdrToAddToTable, std::unordered_map<std::string, OperatorInfoObj>& a_tableToUpdate)
: m_mccmnc(a_mccmnc)
, m_cdrToAddToTable(a_cdrToAddToTable)
, m_tableToUpdate(a_tableToUpdate){
}


void nm::cdr::OperatorTask::Execute() {
    // Check if related InfoObj already exists
    if(this->m_tableToUpdate.find(this->m_mccmnc) == this->m_tableToUpdate.end()) {
        OperatorInfoObj newOperatorObj;
        this->UpdateOperatorInfoObjectAccordingCdr(newOperatorObj);
        this->m_tableToUpdate[this->m_mccmnc] = newOperatorObj;
    }
    else {
        this->UpdateOperatorInfoObjectAccordingCdr(this->m_tableToUpdate.at(this->m_mccmnc));
    }
}

void nm::cdr::OperatorTask::UpdateOperatorInfoObjectAccordingCdr(OperatorInfoObj& a_operatorInfoObj) const {
    switch(this->m_cdrToAddToTable.m_type) {
    case Cdr::MOC: {
        a_operatorInfoObj.m_totalOutgoingVoiceCallDuration += this->m_cdrToAddToTable.m_duration;
        break;
    }

    case Cdr::MTC: {
        a_operatorInfoObj.m_totalIncomingVoiceCallDuration+= this->m_cdrToAddToTable.m_duration;
        break;
    }

    case Cdr::SMS_MO: {
        a_operatorInfoObj.m_totalOutgoingSms += 1;
        break;
    }

    case Cdr::SMS_MT: {
        a_operatorInfoObj.m_totalIncomingSms += 1;
        break;
    }

    case Cdr::D:
    case Cdr::U:
    case Cdr::B:
    case Cdr::X: {
        break; // Do nothing
    }
    }

}