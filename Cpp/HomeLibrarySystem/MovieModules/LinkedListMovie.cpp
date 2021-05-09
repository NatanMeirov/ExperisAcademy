#include "LinkedListMovie.hpp"
#include <stdexcept> // std::out_of_range
#include "LinkedListNodeMovie.hpp"
#include "Movie.hpp"


static Movie RemoveLinkedListNodeMovie(LinkedListNodeMovie* a_nodeToRemove) {
    Movie MovieToSave = a_nodeToRemove->GetMovie();
    delete a_nodeToRemove;

    return MovieToSave;
}


LinkedListMovie::LinkedListMovie()
: m_head(NULL)
, m_moviesNumber(0) {
}


LinkedListMovie::LinkedListMovie(const LinkedListMovie& a_other)
: m_head(NULL)
, m_moviesNumber(a_other.m_moviesNumber) {
    const LinkedListNodeMovie* currentOtherNode = a_other.m_head;
    while(currentOtherNode) { // If is NULL, then the LinkedList is initialized already - finished
        this->PushTail(currentOtherNode->GetMovie());
        currentOtherNode = currentOtherNode->GetNext();
    }
}


LinkedListMovie& LinkedListMovie::operator=(const LinkedListMovie& a_other) {
    const LinkedListNodeMovie* currentNode = this->m_head;

    // Delete current list nodes
    while(currentNode) {
        const LinkedListNodeMovie* nextNode = currentNode->GetNext();
        delete currentNode;
        currentNode = nextNode;
    }

    // Copy other's list nodes
    const LinkedListNodeMovie* currentOtherNode = a_other.m_head;
    while(currentOtherNode) {
        this->PushTail(currentOtherNode->GetMovie());
        currentOtherNode = currentOtherNode->GetNext();
    }

    this->m_moviesNumber = a_other.m_moviesNumber;

    return *this;
}


LinkedListMovie::~LinkedListMovie() {
    LinkedListNodeMovie* currentNodeToDestroy = this->m_head;
    while(currentNodeToDestroy) {
        LinkedListNodeMovie* nextNodeToDestroy = currentNodeToDestroy->GetNext();
        delete currentNodeToDestroy;
        currentNodeToDestroy = nextNodeToDestroy;
    }
}


void LinkedListMovie::PushTail(const Movie& a_movie) {
    if(!this->m_head) { // Check if is the first node in the list
        this->PushHead(a_movie);

        return;
    }

    ++this->m_moviesNumber;

    LinkedListNodeMovie* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    currentNode->SetNext(new LinkedListNodeMovie(a_movie)); // The new node will be initialized with NULL next pointer
}


void LinkedListMovie::PushHead(const Movie& a_movie) {
    ++this->m_moviesNumber;
    this->m_head = new LinkedListNodeMovie(a_movie, this->m_head); // Would work even if head == NULL
}


Movie LinkedListMovie::PopHead() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    --this->m_moviesNumber;

    LinkedListNodeMovie* currentHead = this->m_head;
    this->m_head = this->m_head->GetNext(); // Would work even if next is NULL ( => will be an empty list)

    return RemoveLinkedListNodeMovie(currentHead);
}


Movie LinkedListMovie::PopTail() {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    --this->m_moviesNumber;

    if(!this->m_head->GetNext()) { // It is the head, and there are not additional items in the list
        return this->PopHead();
    }

    LinkedListNodeMovie* currentNode = this->m_head;

    while(currentNode->GetNext()->GetNext()) {
        currentNode = currentNode->GetNext();
    }
    LinkedListNodeMovie* newTailNode = currentNode;
    currentNode = currentNode->GetNext(); // Points the tail node to delete
    newTailNode->SetNext(NULL);

    return RemoveLinkedListNodeMovie(currentNode);
}


Movie LinkedListMovie::GetHead() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    return this->m_head->GetMovie();
}


Movie LinkedListMovie::GetTail() const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    LinkedListNodeMovie* currentNode = this->m_head;
    while(currentNode->GetNext()) {
        currentNode = currentNode->GetNext();
    }

    return currentNode->GetMovie();
}


Movie LinkedListMovie::GetMovieAt(size_t a_index) const {
    if(!this->m_head) {
        throw std::out_of_range("Empty Container Error");
    }

    if(a_index == 0 || this->m_moviesNumber < a_index) {
        throw std::out_of_range("Index Out Of Bounds Error");
    }

    LinkedListNodeMovie* currentNode = this->m_head;

    for(size_t i = 1; i < a_index; ++i) {
        currentNode = currentNode->GetNext();
    }

    return currentNode->GetMovie();
}


const size_t LinkedListMovie::Size() const {
    return this->m_moviesNumber;
}
