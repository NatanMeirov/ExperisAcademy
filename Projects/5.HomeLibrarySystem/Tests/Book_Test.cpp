#include "../BookModules/Book.hpp"
#include <iostream> // std::cout, std::endl


int main() {

    Book book("MyBook", "Me", 2021, "06/05/2021");

    std::cout << book.GetBookName() << std::endl;
    std::cout << book.GetAuthor() << std::endl;
    std::cout << book.GetReleaseYear() << std::endl;
    std::cout << book.GetReceivedDate() << std::endl;

    return 0;
}