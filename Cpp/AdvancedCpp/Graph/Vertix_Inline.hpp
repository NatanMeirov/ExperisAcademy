#ifndef __NM_GRAPH_VERTIX_INLINE_HPP__
#define __NM_GRAPH_VERTIX_INLINE_HPP__


#include "Vertix.hpp"
#include <stdexcept> // std::invalid_argument
#include <algorithm> // std::find_if, std::copy


namespace nm {

namespace graph {

template <typename VertixNameT, typename VertixDataT>
Vertix<VertixNameT,VertixDataT>::Vertix(const VertixNameT& a_vertixName, const VertixDataT& a_vertixData)
: m_vertixName(a_vertixName)
, m_vertixData(a_vertixData)
, m_adjacentVertices() {
}


template <typename VertixNameT, typename VertixDataT>
VertixNameT Vertix<VertixNameT,VertixDataT>::GetName() const {
    return this->m_vertixName;
}


template <typename VertixNameT, typename VertixDataT>
VertixDataT Vertix<VertixNameT,VertixDataT>::GetData() const {
    return this->m_vertixData;
}


template <typename VertixNameT, typename VertixDataT>
typename Vertix<VertixNameT,VertixDataT>::VerticesCollection Vertix<VertixNameT,VertixDataT>::GetAdjacentVertices() const {
    VerticesCollection newCollection = std::list<Vertix<VertixNameT,VertixDataT>*>();
    std::copy(this->m_adjacentVertices.begin(), this->m_adjacentVertices.end(), newCollection); // Would work even if this->list is empty

    return newCollection;
}


template <typename VertixNameT, typename VertixDataT>
void Vertix<VertixNameT,VertixDataT>::AddAdjacent(Vertix<VertixNameT,VertixDataT>* a_adjacentVertix) {
    if(!a_adjacentVertix) {
        throw std::invalid_argument("Null Pointer Error");
    }

    VertixNameT newVertixName = a_adjacentVertix->GetName();
    typename VerticesCollection::iterator endItr = this->m_adjacentVertices.end();
    typename VerticesCollection::iterator findResult = std::find_if(this->m_adjacentVertices.begin(), endItr, [newVertixName](Vertix<VertixNameT,VertixDataT>* a_vertix){return newVertixName == a_vertix->GetName(); });
    if(findResult != endItr) {
        throw std::invalid_argument("Element Already Exists Error");
    }

    this->m_adjacentVertices.push_back(a_adjacentVertix);
}


template <typename VertixNameT, typename VertixDataT>
void Vertix<VertixNameT,VertixDataT>::RemoveAdjacent(const VertixNameT& a_adjacentVertixName) {
    typename std::list<Vertix<VertixNameT,VertixDataT>*>::iterator endItr = this->m_adjacentVertices.end();
    typename std::list<Vertix<VertixNameT,VertixDataT>*>::iterator findResult = std::find_if(this->m_adjacentVertices.begin(), endItr, [a_adjacentVertixName](Vertix<VertixNameT,VertixDataT>* a_vertix){return a_adjacentVertixName == a_vertix->GetName(); });
    if(findResult == endItr) {
        throw std::invalid_argument("Element Not Found Error");
    }

    this->m_adjacentVertices.erase(findResult);
}


} // graph

} // nm


#endif // __NM_GRAPH_VERTIX_INLINE_HPP__