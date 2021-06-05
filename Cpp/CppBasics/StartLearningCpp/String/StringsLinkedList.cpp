#include "StringsLinkedList.hpp"
#include <stdexcept> // std::out_of_range
#include "StringsLinkedListNode.hpp"
#include "String.hpp"


static String RemoveStringsLinkedListNode(StringsLinkedListNode* a_nodeToRemove) {
    String stringToSave = a_nodeToRemove->GetString();
    delete a_nodeToRemove;

    return stringToSave;
}


StringsLinkedList::StringsLinkedList()
: m_head(NULL) {
}


StringsLinkedList::StringsLinkedList(const StringsLinkedList& a_other)
: m_head(NULL) {
    const StringsLinkedListNode* currentOtherNode = a_other.m_head;
    while(currentOtherNode) { // If is NULL, then the LinkedList is initialized already - finished
        this->PushTail(currentOtherNode->GetString());
        currentOtherNode = currentOtherNode->GetNext();
    }
}


StringsLinkedList& StringsLinkedList::operator=(const StringsLinkedList& a_other) {
    const StringsLinkedListNode* currentNode = this->m_head;

    // Delete current list nodes
    while(currentNode) {
        const StringsLinkedListNode* nextNode = currentNode->GetNext();
        delete currentNode;
        currentNode = nextNode;
    }

    // Copy other's list nodes
    const StringsLinkedListNode* currentOtherNode = a_other.m_head;
    while(currentOtherNode) {
        this->PushTail(currentOtherNode->GetString());
        currentOtherNode = currentOtherNode->GetNext();
    }

    return *this;
}


StringsLinkedList::~StringsLinkedList() {
    StringsLinkedListNode* currentNodeToDestroy = this->m_head;
    while(currentNodeToDestroy) {
        StringsLinkedListNode* nextNodeToDestroy = currentNodeToDestroy->GetNext();
        delete currentNodeToDestroy;
        currentNodeToDestroy = nextNodeToDestroy;
    }
}


void StringsLinkedList::PushTail(const String& a_string) {
    if(!this->m_head) { // Check if is the first node in the list
        this->PushHead(a_string);

        return;
    }

    StringsLinkedListNode* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    currentNode->SetNext(new StringsLinkedListNode(a_string)); // The new node will be initialized with NULL next pointer
}


void StringsLinkedList::PushHead(const String& a_string) {
    this->m_head = new StringsLinkedListNode(a_string, this->m_head); // Would work even if head == NULL
}


String StringsLinkedList::PopHead() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    StringsLinkedListNode* currentHead = this->m_head;
    this->m_head = this->m_head->GetNext(); // Would work even if next is NULL ( => will be an empty list)

    return RemoveStringsLinkedListNode(currentHead);
}


String StringsLinkedList::PopTail() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    StringsLinkedListNode* currentNode = this->m_head;

    if(!currentNode->GetNext()) { // It is the head, and there are not additional items in the list
        return this->PopHead();
    }


    while(currentNode->GetNext()->GetNext()) {
        currentNode = currentNode->GetNext();
    }
    StringsLinkedListNode* newTailNode = currentNode;
    currentNode = currentNode->GetNext(); // Points the tail node to delete
    newTailNode->SetNext(NULL);

    return RemoveStringsLinkedListNode(currentNode);
}


String StringsLinkedList::GetHead() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    return this->m_head->GetString();
}


String StringsLinkedList::GetTail() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    StringsLinkedListNode* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    return currentNode->GetString();
}