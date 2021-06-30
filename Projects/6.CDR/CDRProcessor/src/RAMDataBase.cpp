#include "../inc/RAMDataBase.hpp"
#include <cstddef> // size_t
#include <cstdint>
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/Multithreaded/ICommand.hpp"
#include "../inc/TaskFactory.hpp"


nm::cdr::RAMDataBase::RAMDataBase()
: m_billingInfoTable()
, m_operatorSettlementTable()
, m_linkGraphTable()
, m_threadPool(RAMDataBase::THREADS_NUMBER, RAMDataBase::WORKING_TASKS_QUEUE_SIZE) {
}


bool nm::cdr::RAMDataBase::Save(const std::string &a_databaseDirectoryPath) {
    // 1. Open the dir at the given path
    // 2. for each inner directory:
    //      a. open data file
    //      b. deserialize the data from the matching map (table) and write to the data file
    return true;
}


bool nm::cdr::RAMDataBase::Load(const std::string &a_databaseDirectoryPath) {
    // 1. Open the dir at the given path
    // 2. for each inner directory:
    //      a. open data file
    //      b. serialize the data from the matching file and store the data to the matching map (table)
    return true;
}


const nm::cdr::InfoObj* nm::cdr::RAMDataBase::Get(const std::string& a_query, InfoObjOption a_option) {
    switch(a_option) {
    case IDataBase::BILLING: {
        return &this->m_billingInfoTable.at(std::stoul(a_query));
    }

    case IDataBase::OPERATOR: {
        return &this->m_operatorSettlementTable.at(a_query);
    }

    case IDataBase::LINKGRAPH: {
        return &this->m_linkGraphTable.at(std::stoul(a_query));
    }
    }
}


bool nm::cdr::RAMDataBase::Update(const std::string& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Delete(const std::string& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Add(const std::string& a_query, Cdr& a_dataToAdd) {
    ICommand* billingTask = TaskFactory::CreateBillingTask(std::stoul(a_query), a_dataToAdd, this->m_billingInfoTable);
    ICommand* operatorTask = TaskFactory::CreateOperatorTask(a_query, a_dataToAdd, this->m_operatorSettlementTable);
    ICommand* linkgraphTask = TaskFactory::CreateLinkGraphTask(std::stoul(a_query), a_dataToAdd, this->m_linkGraphTable);

    this->m_threadPool.PushTask(billingTask);
    this->m_threadPool.PushTask(operatorTask);
    this->m_threadPool.PushTask(linkgraphTask);
    
    // TODO: find a way to handle the need of deleting the ICommand pointers, AFTER the execution of the tasks, and NOT before...

    return true;
}
