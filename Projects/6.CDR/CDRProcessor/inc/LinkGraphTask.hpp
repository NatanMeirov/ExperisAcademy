#ifndef __NM_CDR_LINKGRAPHTASK_HPP__
#define __NM_CDR_LINKGRAPHTASK_HPP__


#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class LinkGraphTask : public ICommand {
public:
    LinkGraphTask();
    virtual ~LinkGraphTask() = default;

    virtual void Execute() override;

private:

};

} // cdr

} // nm


#endif // __NM_CDR_LINKGRAPHTASK_HPP__