#include "LinkedListBook.hpp"
#include <stdexcept> // std::out_of_range
#include "LinkedListNodeBook.hpp"
#include "Book.hpp"


static Book RemoveLinkedListNodeBook(LinkedListNodeBook* a_nodeToRemove) {
    Book BookToSave = a_nodeToRemove->GetBook();
    delete a_nodeToRemove;

    return BookToSave;
}


LinkedListBook::LinkedListBook()
: m_head(NULL)
, m_booksNumber(0) {
}


LinkedListBook::LinkedListBook(const LinkedListBook& a_other)
: m_head(NULL)
, m_booksNumber(a_other.m_booksNumber) {
    const LinkedListNodeBook* currentOtherNode = a_other.m_head;
    while(currentOtherNode) { // If is NULL, then the LinkedList is initialized already - finished
        this->PushTail(currentOtherNode->GetBook());
        currentOtherNode = currentOtherNode->GetNext();
    }
}


LinkedListBook& LinkedListBook::operator=(const LinkedListBook& a_other) {
    const LinkedListNodeBook* currentNode = this->m_head;

    // Delete current list nodes
    while(currentNode) {
        const LinkedListNodeBook* nextNode = currentNode->GetNext();
        delete currentNode;
        currentNode = nextNode;
    }

    // Copy other's list nodes
    const LinkedListNodeBook* currentOtherNode = a_other.m_head;
    while(currentOtherNode) {
        this->PushTail(currentOtherNode->GetBook());
        currentOtherNode = currentOtherNode->GetNext();
    }

    this->m_booksNumber = a_other.m_booksNumber;

    return *this;
}


LinkedListBook::~LinkedListBook() {
    LinkedListNodeBook* currentNodeToDestroy = this->m_head;
    while(currentNodeToDestroy) {
        LinkedListNodeBook* nextNodeToDestroy = currentNodeToDestroy->GetNext();
        delete currentNodeToDestroy;
        currentNodeToDestroy = nextNodeToDestroy;
    }
}


void LinkedListBook::PushTail(const Book& a_book) {
    if(!this->m_head) { // Check if is the first node in the list
        this->PushHead(a_book);

        return;
    }

    ++this->m_booksNumber;

    LinkedListNodeBook* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    currentNode->SetNext(new LinkedListNodeBook(a_book)); // The new node will be initialized with NULL next pointer
}


void LinkedListBook::PushHead(const Book& a_book) {
    ++this->m_booksNumber;
    this->m_head = new LinkedListNodeBook(a_book, this->m_head); // Would work even if head == NULL
}


Book LinkedListBook::PopHead() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    --this->m_booksNumber;

    LinkedListNodeBook* currentHead = this->m_head;
    this->m_head = this->m_head->GetNext(); // Would work even if next is NULL ( => will be an empty list)

    return RemoveLinkedListNodeBook(currentHead);
}


Book LinkedListBook::PopTail() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    --this->m_booksNumber;

    if(!this->m_head->GetNext()) { // It is the head, and there are not additional items in the list
        return this->PopHead();
    }

    LinkedListNodeBook* currentNode = this->m_head;

    while(currentNode->GetNext()->GetNext()) {
        currentNode = currentNode->GetNext();
    }
    LinkedListNodeBook* newTailNode = currentNode;
    currentNode = currentNode->GetNext(); // Points the tail node to delete
    newTailNode->SetNext(NULL);

    return RemoveLinkedListNodeBook(currentNode);
}


Book LinkedListBook::GetHead() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    return this->m_head->GetBook();
}


Book LinkedListBook::GetTail() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    LinkedListNodeBook* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    return currentNode->GetBook();
}


Book LinkedListBook::GetBookAt(size_t a_index) const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    if(a_index == 0 || this->m_booksNumber < a_index) {
        throw std::out_of_range("Index Out Of Bounds Error");
    }

    LinkedListNodeBook* currentNode = this->m_head;

    for(size_t i = 1; i < a_index; ++i) {
        currentNode = currentNode->GetNext();
    }

    return currentNode->GetBook();
}


const size_t LinkedListBook::Size() const {
    return this->m_booksNumber;
}
