#include "StringsLinkedListNode.hpp"
#include "String.hpp"


StringsLinkedListNode::StringsLinkedListNode(const String& a_string)
: m_string(a_string)
, m_next(NULL) {
}


StringsLinkedListNode::StringsLinkedListNode(const String& a_string, StringsLinkedListNode* a_next)
: m_string(a_string)
, m_next(a_next) {
}


StringsLinkedListNode::StringsLinkedListNode(const StringsLinkedListNode& a_other)
: m_string(a_other.m_string)
, m_next(a_other.m_next) {
}


StringsLinkedListNode& StringsLinkedListNode::operator=(const StringsLinkedListNode& a_other) {
    this->m_string = a_other.m_string;
    this->m_next = a_other.m_next;

    return *this;
}


StringsLinkedListNode::~StringsLinkedListNode() {
}


void StringsLinkedListNode::SetNext(StringsLinkedListNode* a_next) {
    this->m_next = a_next;
}


String StringsLinkedListNode::GetString() const {
    return this->m_string;
}


StringsLinkedListNode* StringsLinkedListNode::GetNext() const {
    return this->m_next;
}