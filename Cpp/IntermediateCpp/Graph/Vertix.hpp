#ifndef __NM_GRAPH_VERTIX_HPP__
#define __NM_GRAPH_VERTIX_HPP__


#include <list>


namespace nm {

namespace graph {

// CONCEPT: VertixNameT AND VertixDataT MUST be copy constructable and copy assignable
template <typename VertixNameT, typename VertixDataT>
class Vertix {
public:
    typedef std::list<Vertix<VertixNameT,VertixDataT>*> VerticesCollection;

    Vertix(const VertixNameT& a_vertixName, const VertixDataT& a_vertixData);
    Vertix(const Vertix<VertixNameT,VertixDataT>& a_other) = default;
    Vertix<VertixNameT,VertixDataT>& operator=(const Vertix<VertixNameT,VertixDataT>& a_other) = default;
    ~Vertix() = default;

    // Const Methods
    VertixNameT GetName() const;
    VertixDataT GetData() const;
    VerticesCollection GetAdjacentVertices() const;

    // Methods
    void AddAdjacent(Vertix<VertixNameT,VertixDataT>* a_adjacentVertix);
    void RemoveAdjacent(const VertixNameT& a_adjacentVertixName);

private:
    VertixNameT m_vertixName;
    VertixDataT m_vertixData;
    VerticesCollection m_adjacentVertices;
};

} // graph

} // nm


#endif // __NM_GRAPH_VERTIX_HPP__