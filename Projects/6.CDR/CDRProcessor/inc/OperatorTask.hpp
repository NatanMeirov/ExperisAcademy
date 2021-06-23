#ifndef __NM_CDR_OPERATORTASK_HPP__
#define __NM_CDR_OPERATORTASK_HPP__


#include "../../Infrastructure/Multithreaded/ICommand.hpp"


namespace nm {

namespace cdr {

class OperatorTask : public ICommand {
public:
    OperatorTask();
    virtual ~OperatorTask() = default;

    virtual void Execute() override;

private:

};

} // cdr

} // nm


#endif // __NM_CDR_OPERATORTASK_HPP__