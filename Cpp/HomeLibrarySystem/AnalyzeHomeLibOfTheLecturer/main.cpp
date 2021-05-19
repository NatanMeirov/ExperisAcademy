#include <list> // std::list
#include <iostream> // std::cout, std::endl
#include "movie.hpp"
#include "book.hpp"


homelib::Book g_harryPotter("Harry Potter", "J.K. Rawling", 1998);
homelib::Movie g_harryPotterMovie("Harry Potter", homelib::Movie::FANTASY, 2002);
homelib::Book g_warAndPeace("War and Peace", "Leo Tolstoy", 1869);


void ActualItems() {
    std::cout << "Actual Book: ";
    g_harryPotter.Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!) //! Natan: The Book's print method is called: Book_Print(&g_harryPotter, ...); - the reason is: g_harryPotter is a Book instance, and can use all public methods of the Book's class
    std::cout << std::endl;

    std::cout << "Actual Movie: ";
    g_harryPotterMovie.Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!) //! Natan: The Movie's print method is called: Movie_Print(&g_harryPotterMovie, ...); - the reason is: g_harryPotterMovie is a Movie instance, and can use all public methods of the Movie's class
    std::cout << std::endl;

    std::cout << "Book copy: ";
    homelib::Book harryPotterCopy(g_harryPotter);
    harryPotterCopy.Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!) //! Natan: The Book's print method is called: Book_Print(&harryPotterCopy, ...); - the reason is: harryPotterCopy is a Book instance, and can use all public methods of the Book's class
    std::cout << std::endl;

    std::cout << "NOT a book copy: ";
    homelib::LibraryItem notBook(g_harryPotter);
    notBook.Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!) //! Natan: The LibraryItem's print method is called: LibraryItem_Print(&notBook, ...); - the reason is: notBook is not a Book instance, but LibraryItem instance (that initialized from the LIBRARYITEM'S PART OF BOOK CLASS), and can use only public methods of the LibraryItem's class
    std::cout << std::endl;

    std::cout << "Book, but weird Print: ";
    g_harryPotter.LibraryItem::Print(std::cout); // QUESTION: What am I asking the compiler to do? //! Natan: asking the compiler to use the LibraryItem's inner part of Book to use its print on the data
    std::cout << std::endl;
}


void UsingPointers() {
    homelib::Book *harryPotterPtr(&g_harryPotter);
    homelib::Movie *moviePtr(&g_harryPotterMovie);
    std::cout << "Book pointer: ";
    harryPotterPtr->Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!)
    std::cout << std::endl;

    std::cout << "Movie pointer: ";
    moviePtr->Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!)
    std::cout << std::endl;

    std::cout << "Book BASE pointer: ";
    homelib::LibraryItem *baseOfBook(&g_harryPotter);
    baseOfBook->Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!)
    std::cout << std::endl;

    std::cout << "Movie BASE pointer: ";
    homelib::LibraryItem *baseOfMovie(&g_harryPotterMovie);
    baseOfMovie->Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!)
    std::cout << std::endl;

    std::cout << "Movie pointer, again: ";
    homelib::Movie *movieAgain = (homelib::Movie*)baseOfMovie;
    movieAgain->Print(std::cout); // QUESTION: Which Print() is called? Why? (Write the rewrite!)
    std::cout << std::endl;

    // Let's lie...
    std::cout << "Movie pointer, lie: ";
    homelib::Movie *bigLie = (homelib::Movie*)baseOfBook;
    bigLie->Print(std::cout); // QUESTION: !!!! What happens here? Look at the publication year!
    std::cout << std::endl;
}


void ListOfPointers() {
    typedef std::list<homelib::LibraryItem*> ListType;
    ListType items;
    std::cout << std::endl << "List of Pointers" << std::endl;
    std::cout << "----------------" << std::endl;
    items.push_back(&g_harryPotter);
    items.push_back(&g_harryPotterMovie);
    items.push_back(&g_warAndPeace);

    for (ListType::iterator itr = items.begin(); itr != items.end(); ++itr) {
        ListType::value_type curPtr = *itr; // QUESTION: What is the type of ListType::value_type?
        homelib::LibraryItem &curRef = *curPtr; // QUESTION: What does the reference mean? What happens if you make a copy?
        curRef.Print(std::cout); // QUESTION: Which Print() is called? Why? (write what the compiler rewrites this to!)
        std::cout << std::endl;
    }

    std::cout << "First item is a book: ";
    homelib::LibraryItem *firstItem = *items.begin();
    homelib::Book *firstBook = (homelib::Book*)firstItem;
    firstBook->Print(std::cout); // QUESTION: Does this work correctly? Why?
    std::cout << std::endl;
}


void ListOfBase() {
    typedef std::list<homelib::LibraryItem> ListType; // QUESTION: How is this different than a list of pointers?
    ListType items;

    std::cout << std::endl << "List of Base" << std::endl;
    std::cout << "----------------" << std::endl;
    items.push_back(g_harryPotter);
    items.push_back(g_harryPotterMovie);
    items.push_back(g_warAndPeace);

    for (ListType::iterator itr = items.begin(); itr != items.end(); ++itr) {
        ListType::value_type &cur = *itr; // QUESTION: What is the type of value_type here?
        cur.Print(std::cout); // QUESTION: Which Print() is called? Why? (write what the compiler rewrites this to!)
        std::cout << std::endl;
    }

    std::cout << "First item is a book? ";
    homelib::LibraryItem &firstItem = *items.begin();
    homelib::Book &firstBook = (homelib::Book&)firstItem;
    firstBook.Print(std::cout); // QUESTION: Does this work correctly? Why?
    std::cout << std::endl;
}


int main() {
    ActualItems();
    // UsingPointers();
    // ListOfPointers();
    // ListOfBase();
}