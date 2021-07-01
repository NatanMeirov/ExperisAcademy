#ifndef NM_META_TUPLE_UNPACK
#define NM_META_TUPLE_UNPACK


#include <cstddef> // size_t
#include <tuple> // std::tuple_size, std::get, std::tuple_size


namespace nm
{

namespace meta
{

// A sequence holder (to be used with sizeof...(Args) - to count the number of the variadic template [0..N]) {Example: Sequence<> is empty}
template <size_t ...>
struct Sequence
{
};


// A Sequence "expander", to extract the sequence in each instantiation (...Seq can be none) -> SequenceExpander<N> is valid {Example: SequenceExpander<3>::type == Sequence<0, 1, 2>} [Reference to deeper explanation: https://stackoverflow.com/questions/36612596/tuple-to-parameter-pack]
template <size_t N, size_t ...Seq>
struct SequenceExpander : public SequenceExpander<N - 1, N - 1, Seq...>
{
};


// A SequenceExpander specialization to be used as a stopping condition - all other SequenceExpanders inherits from it (in the top of that inheritance list), so type is declared inside them as well
template <size_t ...Seq>
struct SequenceExpander<0, Seq...>
{
    typedef Sequence<Seq...> type;
};


template <typename RetT = void, typename Func, typename Tuple, size_t ...Seq>
RetT ExpandedFuncCall(Func a_func, Tuple a_tuple)
{
    return a_func(std::get<Seq>(a_tuple) ...);
}


template <typename RetT = void, typename Func, typename ...Args>
RetT UnpackTupleAndCallFunc(Func a_func, std::tuple<Args...> a_tuple)
{
    return ExpandedFuncCall<RetT, Func, SequenceExpander<std::tuple_size<std::tuple<Args...>>::value>::type>(a_func, a_tuple);
};

} // meta

} // nm


#endif // NM_META_TUPLE_UNPACK