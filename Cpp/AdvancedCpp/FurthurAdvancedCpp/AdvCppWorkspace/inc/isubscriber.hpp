#ifndef NM_ISUBSCRIBER_HPP
#define NM_ISUBSCRIBER_HPP


namespace smartbuilding
{

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;
    virtual void Notify() = 0;
};

} // smartbuilding


#endif // NM_ISUBSCRIBER_HPP