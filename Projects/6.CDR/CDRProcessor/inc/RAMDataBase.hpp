#ifndef __NM_CDR_RAMDATABASE_HPP__
#define __NM_CDR_RAMDATABASE_HPP__


#include <cstddef> // size_t
#include <unordered_map>
#include "IDataBase.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/Multithreaded/ThreadPool.hpp"
#include "../../Infrastructure/inc/InfoObj.hpp"
#include "../../Infrastructure/inc/BillingInfoObj.hpp"
#include "../../Infrastructure/inc/OperatorInfoObj.hpp"
#include "../../Infrastructure/inc/LinkGraphInfoObj.hpp"


namespace nm {

namespace cdr {

class RAMDataBase : public IDataBase {
public:
    RAMDataBase();

    virtual bool Load(const std::string& a_databaseDirectoryPath) override;
    virtual bool Save(const std::string& a_databaseDirectoryPath) override;
    virtual const InfoObj* Get(const std::string& a_query, InfoObjOption a_option) override;
    virtual bool Update(const std::string& a_query) override;
    virtual bool Delete(const std::string& a_query) override;
    virtual bool Add(const std::string& a_query, Cdr& a_dataToAdd) override; // Use task factory to create new 3 tasks (to add this single cdr to all 3 data-maps)

private:
    static const unsigned int THREADS_NUMBER = 4;
    static const unsigned int WORKING_TASKS_QUEUE_SIZE = 39;

    std::unordered_map<uint64_t, BillingInfoObj> m_billingInfoTable; // Key: IMSI
    std::unordered_map<std::string, OperatorInfoObj> m_operatorSettlementTable; // Key: MCC+MNC (operator info)
    std::unordered_map<uint64_t, LinkGraphInfoObj> m_linkGraphTable; // Key: IMSI
    nm::runtime::ThreadPool m_threadPool;
};

} // cdr

} // nm


#endif // __NM_CDR_RAMDATABASE_HPP__