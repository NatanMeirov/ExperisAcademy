#ifndef NM_ICALLABLE_HPP
#define NM_ICALLABLE_HPP


namespace advcpp
{

// An abstract polymorphic base for a Callable function object [void(void)]
class ICallable
{
public:
    virtual ~ICallable() = default;
    virtual void operator()() = 0;
};

} // advcpp


#endif // NM_ICALLABLE_HPP
