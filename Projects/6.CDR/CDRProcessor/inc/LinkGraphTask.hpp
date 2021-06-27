#ifndef __NM_CDR_LINKGRAPHTASK_HPP__
#define __NM_CDR_LINKGRAPHTASK_HPP__


#include <unordered_map>
#include "../../Infrastructure/inc/Cdr.hpp"
#include "../../Infrastructure/inc/LinkGraphInfoObj.hpp"
#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class LinkGraphTask : public ICommand {
public:
    LinkGraphTask(const uint64_t a_imsi, const Cdr& a_drToAddToTable, std::unordered_map<uint64_t, LinkGraphInfoObj>& a_tableToUpdate);
    virtual ~LinkGraphTask() = default;

    virtual void Execute() override;

private:
    uint64_t m_imsi;
    Cdr m_cdrToAddToTable;
    std::unordered_map<uint64_t, LinkGraphInfoObj>& m_tableToUpdate;
};

} // cdr

} // nm


#endif // __NM_CDR_LINKGRAPHTASK_HPP__