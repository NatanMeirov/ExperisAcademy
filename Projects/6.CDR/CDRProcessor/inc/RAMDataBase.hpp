#ifndef __NM_CDR_RAMDATABASE_HPP__
#define __NM_CDR_RAMDATABASE_HPP__


#include <cstddef> // size_t
#include <unordered_map>
#include "IDataBase.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/Multithreaded/SafeQueue.hpp" // Tasks safe queue
#include "BillingInfoObj.hpp"
#include "OperatorInfoObj.hpp"
#include "LinkGraphInfoObj.hpp"


namespace nm {

namespace cdr {

class RAMDataBase : public IDataBase {
public:
    virtual bool Load(const std::string& a_databaseFileNamePath) override;
    virtual bool Save(const std::string& a_databaseFileNamePath) override;
    virtual Cdr Get(const uint64_t& a_query) override;
    virtual bool Update(const uint64_t& a_query) override;
    virtual bool Delete(const uint64_t& a_query) override;
    virtual bool Add(const uint64_t& a_query, const Cdr& a_dataToAdd) override; // use task factory to create new 3 tasks (to add this single cdr to all 3 data-maps)

private:
    std::unordered_map<uint64_t, BillingInfoObj> m_billingInfoDataBase;
    std::unordered_map<uint64_t, OperatorInfoObj> m_operatorSettlementDataBase;
    std::unordered_map<uint64_t, LinkGraphInfoObj> m_linkGraphDataBase;
    // nm::SafeQueue<Task> m_pendingTasks; class Task : public ICommand { implement Execute() } => and use this way: vector<ICommand> a (for single : a) { single->Execute(); }
};

} // cdr

} // nm


#endif // __NM_CDR_RAMDATABASE_HPP__