#include "../inc/RAMDataBase.hpp"
#include <cstddef> // size_t
#include <cstdint>
#include "../../Infrastructure/inc/Cdr.hpp"


nm::cdr::RAMDataBase::RAMDataBase()
: m_billingInfoDataBase()
, m_operatorSettlementDataBase()
, m_linkGraphDataBase()
, m_threadPool(RAMDataBase::THREADS_NUMBER, RAMDataBase::WORKING_ITEMS_QUEUE_SIZE) {
}


bool nm::cdr::RAMDataBase::Save(const std::string &a_databaseFileNamePath) {
    // TODO
    return true;
}


bool nm::cdr::RAMDataBase::Load(const std::string &a_databaseFileNamePath) {
    // TODO
    return true;
}

nm::cdr::Cdr nm::cdr::RAMDataBase::Get(const uint64_t& a_query) {
    // TODO
    return Cdr();
}


bool nm::cdr::RAMDataBase::Update(const uint64_t& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Delete(const uint64_t& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Add(const uint64_t& a_query, Cdr& a_dataToAdd) {
    // TODO - use threads pool and update ALL 3 inner maps (each of them with the referring relevant data [extracted from the given cdr])
    return true;
}