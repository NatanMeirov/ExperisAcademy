#include "../Include/Book.hpp"
#include <string> // std::string
#include "../Include/ArtWork.hpp"


Book::Book(const std::string& a_bookName, const std::string& a_authorName, const unsigned int& a_publishYear)
: ArtWork(a_bookName, a_authorName, a_publishYear, std::string("Book")) {
}


Book::Book(const Book& a_other)
: ArtWork(a_other){
}


Book& Book::operator=(const Book &a_other) {
    this->ArtWork::operator=(a_other);

    return *this;
}


Book::~Book() {
}