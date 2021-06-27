#ifndef __NM_CDR_OPERATORTASK_HPP__
#define __NM_CDR_OPERATORTASK_HPP__


#include <unordered_map>
#include "../../Infrastructure/inc/OperatorInfoObj.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class OperatorTask : public ICommand {
public:
    OperatorTask(const std::string& a_mccmnc, const Cdr& a_cdrToAddToTable, std::unordered_map<std::string, OperatorInfoObj>& a_tableToUpdate);
    virtual ~OperatorTask() = default;

    virtual void Execute() override;

private:
    std::string m_mccmnc;
    Cdr m_cdrToAddToTable;
    std::unordered_map<std::string, OperatorInfoObj>& m_tableToUpdate;
};

} // cdr

} // nm


#endif // __NM_CDR_OPERATORTASK_HPP__