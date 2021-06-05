#ifndef __NM_GRAPH_GRAPH_HPP__
#define __NM_GRAPH_GRAPH_HPP__


#include "Vertix.hpp"


namespace nm {

namespace graph {

template <typename VertixNameT, typename VertixDataT>
class Graph {
public:
    typedef typename Vertix<VertixNameT,VertixDataT>::VerticesCollection VerticesCollection;
    typedef std::list<VertixNameT> VertixNamesCollection;

    Graph() = default;
    Graph(const Graph<VertixNameT,VertixDataT>& a_other) = default;
    Graph<VertixNameT,VertixDataT>& operator=(const Graph<VertixNameT,VertixDataT>& a_other) = default;
    ~Graph() = default;

    void AddVertix(const VertixNameT& a_vertixName, const VertixDataT& a_vertixData);
    void Connect(const VertixNameT& a_firstVertixName, const VertixNameT& a_secondVertixName);
    VerticesCollection GetAdjacentsOf(const VertixNameT& a_vertixName) const;
    VertixNamesCollection GetAllVerticesNames() const;

private:
    VerticesCollection m_graphVertices;
};

} // graph

} // nm


#endif // __NM_GRAPH_GRAPH_HPP__