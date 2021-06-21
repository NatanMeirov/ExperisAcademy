#ifndef __NM_ICOMMAND_HPP__
#define __NM_ICOMMAND_HPP__


namespace {

// A polymorphic interface of Command design pattern, for commands oriented class objects
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void Execute() = 0;
};

} // nm


#endif // __NM_ICOMMAND_HPP__