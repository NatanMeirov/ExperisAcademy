#ifndef __NM_CDR_RAMDATABASE_HPP__
#define __NM_CDR_RAMDATABASE_HPP__


#include <cstddef> // size_t
#include <unordered_map>
#include "IDataBase.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"


namespace nm {

namespace cdr {

class RAMDataBase : public IDataBase<size_t, Cdr> { // size_t stands for an ID numbers
public:
    virtual bool Load(const std::string& a_databaseFileName) override;
    virtual bool Save(const std::string& a_databaseFileName) override;
    virtual Cdr Get(const size_t& a_query) override;
    virtual bool Update(const size_t& a_query) override;
    virtual bool Delete(const size_t& a_query) override;
    virtual bool Add(const size_t& a_query, const Cdr& _dataToAdd) override;

private:
    // std::unordered_map<size_t, Cdr> m_billingInfoDataBase; //? Cdr or pointer to inner DB of <size_t,T>
    // std::unordered_map<size_t, Cdr> m_operatorSettlementDataBase; //? Cdr or pointer to inner DB of <size_t,OpT>
    // std::unordered_map<size_t, Cdr> m_linkGraphDataBase; //? Cdr or pointer to inner DB of <size_t,otherT>
};

} // cdr

} // nm


#endif // __NM_CDR_RAMDATABASE_HPP__