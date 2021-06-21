#ifndef __NM_CDR_IDATABASE_HPP__
#define __NM_CDR_IDATABASE_HPP__


#include <string> // std::string
#include "../../Infrastructure/inc/Cdr.hpp"


namespace nm{

namespace cdr {

// An interface for a database object (uint64_t stands for an ID numbers)
class IDataBase {
public:
    virtual ~IDataBase() = default;
    virtual bool Load(const std::string& a_databaseFileNamePath) = 0;
    virtual bool Save(const std::string& a_databaseFileNamePath) = 0;

    virtual Cdr Get(const uint64_t& a_query) = 0;
    virtual bool Update(const uint64_t& a_query) = 0;
    virtual bool Delete(const uint64_t& a_query) = 0;
    virtual bool Add(const uint64_t& a_query, Cdr& _dataToAdd) = 0;
};

} // cdr

} // nm


#endif // __NM_CDR_IDATABASE_HPP__