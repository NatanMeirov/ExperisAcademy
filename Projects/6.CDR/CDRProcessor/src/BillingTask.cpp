#include "../inc/BillingTask.hpp"
#include "../../Infrastructure/inc/BillingInfoObj.hpp"


nm::cdr::BillingTask::BillingTask(const uint64_t a_imsi, const Cdr& a_cdrToAddToTable, std::unordered_map<uint64_t, BillingInfoObj>& a_tableToUpdate)
: m_imsi(a_imsi)
, m_cdrToAddToTable(a_cdrToAddToTable)
, m_tableToUpdate(a_tableToUpdate){
}


void nm::cdr::BillingTask::Execute() {
    // Check if related InfoObj already exists
    if(this->m_tableToUpdate.find(this->m_imsi) == this->m_tableToUpdate.end()) {
        BillingInfoObj newBillingObj;
        this->UpdateBillingInfoObjectAccordingCdr(newBillingObj);
        this->m_tableToUpdate[this->m_imsi] = newBillingObj;
    }
    else {
        this->UpdateBillingInfoObjectAccordingCdr(this->m_tableToUpdate.at(this->m_imsi));
    }
}


void nm::cdr::BillingTask::UpdateBillingInfoObjectAccordingCdr(BillingInfoObj& a_billingInfoObj) const {
    switch(this->m_cdrToAddToTable.m_type) {
    case Cdr::MOC: {
        a_billingInfoObj.m_outgoingVoiceCallDuration += this->m_cdrToAddToTable.m_duration;
        break;
    }

    case Cdr::MTC: {
        a_billingInfoObj.m_incomingVoiceCallDuration += this->m_cdrToAddToTable.m_duration;
        break;
    }

    case Cdr::SMS_MO: {
        a_billingInfoObj.m_totalSmsSent += 1;
        break;
    }

    case Cdr::SMS_MT: {
        a_billingInfoObj.m_totalSmsReceived += 1;
        break;
    }

    case Cdr::D: {
        a_billingInfoObj.m_totalDataReceived += this->m_cdrToAddToTable.m_bytesReceived;
        a_billingInfoObj.m_totalDataTransferred += this->m_cdrToAddToTable.m_bytesTransmitted;
        break;
    }

    case Cdr::U:
    case Cdr::B:
    case Cdr::X: {
        break; // Do nothing
    }
    }

    if(this->m_cdrToAddToTable.m_type != Cdr::D) { // Data does not have second party data at all
        if(a_billingInfoObj.m_secondPartiesInfoTable.find(this->m_cdrToAddToTable.m_msisdnOfSecondParty) == a_billingInfoObj.m_secondPartiesInfoTable.end()) {
            SecondPartyInfo secondPartyInfo;
            secondPartyInfo.m_totalVoiceCallDuration = this->m_cdrToAddToTable.m_duration;
            secondPartyInfo.m_totalSmsExchanged = (this->m_cdrToAddToTable.m_type == Cdr::SMS_MO || this->m_cdrToAddToTable.m_type == Cdr::SMS_MT) ? 1 : 0;
            a_billingInfoObj.m_secondPartiesInfoTable[this->m_cdrToAddToTable.m_msisdnOfSecondParty] = secondPartyInfo;
        }
        else { // Already exists
            SecondPartyInfo& secondPartyInfoRef = a_billingInfoObj.m_secondPartiesInfoTable.at(this->m_cdrToAddToTable.m_msisdnOfSecondParty);
            secondPartyInfoRef.m_totalVoiceCallDuration += this->m_cdrToAddToTable.m_duration;
            secondPartyInfoRef.m_totalSmsExchanged += (this->m_cdrToAddToTable.m_type == Cdr::SMS_MO || this->m_cdrToAddToTable.m_type == Cdr::SMS_MT) ? 1 : 0;
        }
    }

}