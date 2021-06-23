#ifndef __NM_CDR_TASKFACTORY_HPP__
#define __NM_CDR_TASKFACTORY_HPP__


#include <string>
#include <unordered_map>
#include "../../Infrastructure/Multithreaded/ICommand.hpp"
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/inc/BillingInfoObj.hpp"
#include "../../Infrastructure/inc/OperatorInfoObj.hpp"
#include "../../Infrastructure/inc/LinkGraphInfoObj.hpp"


namespace nm {

namespace cdr {

class TaskFactory {
public:
    static ICommand* CreateBillingTask(const uint64_t a_imsi, const Cdr& a_cdr, std::unordered_map<uint64_t, BillingInfoObj>& a_table);
    static ICommand* CreateOperatorTask(const std::string& a_mccmnc, const Cdr& a_cdr, std::unordered_map<std::string, OperatorInfoObj>& a_table);
    static ICommand* CreateLinkGraphTask(const uint64_t a_imsi, const Cdr& a_cdr, std::unordered_map<uint64_t, LinkGraphInfoObj>& a_table);
};

} // cdr

} // nm


#endif // __NM_CDR_TASKFACTORY_HPP__