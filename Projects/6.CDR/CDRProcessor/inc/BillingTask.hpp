#ifndef __NM_CDR_BILLINGTASK_HPP__
#define __NM_CDR_BILLINGTASK_HPP__


#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class BillingTask : public ICommand {
public:
    BillingTask();
    virtual ~BillingTask() = default;

    virtual void Execute() override;

private:

};

} // cdr

} // nm


#endif // __NM_CDR_BILLINGTASK_HPP__