#include "../inc/TaskFactory.hpp"
#include "../inc/BillingTask.hpp"
#include "../inc/OperatorTask.hpp"
#include "../inc/LinkGraphTask.hpp"


nm::ICommand* nm::cdr::TaskFactory::CreateBillingTask(const uint64_t a_imsi, const Cdr& a_cdr, std::unordered_map<uint64_t, BillingInfoObj>& a_table) {
    return new BillingTask(a_imsi, a_cdr, a_table);
}


nm::ICommand* nm::cdr::TaskFactory::CreateOperatorTask(const std::string& a_mccmnc, const Cdr& a_cdr, std::unordered_map<std::string, OperatorInfoObj>& a_table) {
    return new OperatorTask(a_mccmnc, a_cdr, a_table);
}


nm::ICommand* nm::cdr::TaskFactory::CreateLinkGraphTask(const uint64_t a_imsi, const Cdr& a_cdr, std::unordered_map<uint64_t, LinkGraphInfoObj>& a_table) {
    return new LinkGraphTask(a_imsi, a_cdr, a_table);
}