#ifndef __NM_GRAPH_GRAPH_INLINE_HPP__
#define __NM_GRAPH_GRAPH_INLINE_HPP__


#include "Graph.hpp"


namespace nm {

namespace graph {

template <typename VertixNameT, typename VertixDataT>
void Graph<VertixNameT,VertixDataT>::AddVertix(const VertixNameT& a_vertixName, const VertixDataT& a_vertixData) {

}


template <typename VertixNameT, typename VertixDataT>
void Graph<VertixNameT,VertixDataT>::Connect(const VertixNameT& a_firstVertixName, const VertixNameT& a_secondVertixName) {

}


template <typename VertixNameT, typename VertixDataT>
typename Vertix<VertixNameT,VertixDataT>::VerticesCollection Graph<VertixNameT,VertixDataT>::GetAdjacentsOf(const VertixNameT& a_vertixName) const {

}


template <typename VertixNameT, typename VertixDataT>
typename Graph<VertixNameT,VertixDataT>::VertixNamesCollection Graph<VertixNameT,VertixDataT>::GetAllVerticesNames() const {

}

} // graph

} // nm


#endif // __NM_GRAPH_GRAPH_INLINE_HPP__