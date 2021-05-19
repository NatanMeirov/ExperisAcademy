#include "../Include/HomeLibrarySystemManager.hpp"
#include <cstddef> // size_t
#include <exception>
#include <stdexcept> // std::invalid_argument
#include <ctime> // time, localtime
#include "../Include/HomeLibrarySystemUIHandler.hpp"
#include "../Include/Book.hpp"
#include "../Include/Movie.hpp"
#include "../Include/CollectableItem.hpp"
#include "../Include/TLibraryIterator_Inline.hpp"


#define BASE_COUNTED_YEAR_TIME 1900


// // A generic CollectableItem factory function
// // Concept: C'tor of item that gets 3 args: [std::string] item's name, [std::string] item's owner name, [unsigned int:] item's publish year
// template <typename T>
// T* CreateNewCollectableItem(const std::string& a_itemName, const std::string& a_itemOwner, const unsigned int& a_itemPublishYear) {
//     return new T(a_itemName, a_itemOwner, a_itemPublishYear);
// }


// Concept: Defined PrintItem method of the given T
template <typename T>
void PrintItemUsingUI(const T& a_item, void* a_uiHandler) {
    static_cast<HomeLibrarySystemUIHandler*>(a_uiHandler)->PrintItem(a_item);
}


void PrintCollectableItem(const CollectableItem& a_item, void* a_uiHandler) {
    if(a_item.GetCollectableItemType() == "Book") {
        // static_cast<HomeLibrarySystemUIHandler*>(a_uiHandler)->PrintBook((Book&)a_item);
        PrintItemUsingUI<Book>(static_cast<const Book&>(a_item), static_cast<HomeLibrarySystemUIHandler*>(a_uiHandler));
    }
    else {
        // static_cast<HomeLibrarySystemUIHandler*>(a_uiHandler)->PrintMovie((Movie&)a_item);
        PrintItemUsingUI<Movie>(static_cast<const Movie&>(a_item), static_cast<HomeLibrarySystemUIHandler*>(a_uiHandler));
    }
}


bool HomeLibrarySystemManager::HandleHomeLibrarySystemUserChoice(HomeLibrarySystemUIHandler::MenuOptions a_option) {
    HomeLibrarySystemUIHandler uiHandler;
    long years;
    std::string subString;

    try {
        switch(a_option) {
        case HomeLibrarySystemUIHandler::ADD_NEW_BOOK:
            this->AddNewItemToLibrary("Book");
            break;

        case HomeLibrarySystemUIHandler::ADD_NEW_MOVIE:
            this->AddNewItemToLibrary("Movie");
            break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_PUBLISHED_IN_LAST_X_YEARS:
        //     years = uiHandler.GetYearsFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&years, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_PUBLISHED_IN_LAST_X_YEARS:
        //     years = uiHandler.GetYearsFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&years, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_COLLECTED_IN_LAST_X_YEARS:
        //     years = uiHandler.GetYearsFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&years, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_COLLECTED_IN_LAST_X_YEARS:
        //     years = uiHandler.GetYearsFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&years, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_FOUND_BY_MATCHING_CHARS:
        //     subString = uiHandler.GetMatchingCharactersFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&subString, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_FOUND_BY_MATCHING_CHARS:
        //     subString = uiHandler.GetMatchingCharactersFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&subString, PrintCollectableItem, (void*)&uiHandler); // NULL - predicate function!
        //     break;

        // case HomeLibrarySystemUIHandler::FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_BOOKS_TO_X_FOUND_BY_MATCHING_CHARS:
        //     subString = uiHandler.GetMatchingCharactersFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&subString, NULL, NULL); // first NULL - predicate function! second NULL - change collected year to x action function! third - context!
        //     break;

        // case HomeLibrarySystemUIHandler::FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_MOVIES_TO_X_FOUND_BY_MATCHING_CHARS:
        //     subString = uiHandler.GetMatchingCharactersFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&subString, NULL, NULL); // first NULL - predicate function! second NULL - change collected year to x action function! third - context!
        //     break;

        default:
            return false;
        }
    }
    catch(const std::exception& ex) {
        uiHandler.ShowHomeLibraryErrorMessage(ex.what());
        return false;
    }

    return true;
}


HomeLibrarySystemManager::HomeLibrarySystemManager()
: m_library() {
}


HomeLibrarySystemManager::HomeLibrarySystemManager(const HomeLibrarySystemManager& a_other)
: m_library(a_other.m_library) {
}

HomeLibrarySystemManager& HomeLibrarySystemManager::operator=(const HomeLibrarySystemManager& a_other) {
    if(this == &a_other) {
        return *this;
    }

    Library<CollectableItem*>::Iterator currentThisIterator = this->m_library.Begin();
    Library<CollectableItem*>::Iterator endThisIterator = this->m_library.End();

    while(currentThisIterator != endThisIterator) {
        delete *currentThisIterator;
        ++currentThisIterator;
    }

    Library<CollectableItem*>::Iterator currentOtherIterator = a_other.m_library.Begin();
    Library<CollectableItem*>::Iterator endOtherIterator = a_other.m_library.End();

    while(currentOtherIterator != endOtherIterator) {
        CollectableItem* newCollectableItem = NULL;

        if((*currentOtherIterator)->GetCollectableItemType() == "Book") {
            Book* currentOtherBook = static_cast<Book*>(*currentOtherIterator);
            newCollectableItem = new Book(*currentOtherBook);
        }
        else { // Movie
            Movie* currentOtherMovie = static_cast<Movie*>(*currentOtherIterator);
            newCollectableItem = new Movie(*currentOtherMovie);
        }

        this->m_library.Add(newCollectableItem);
    }

    return *this;
}


HomeLibrarySystemManager::~HomeLibrarySystemManager() {
    Library<CollectableItem*>::Iterator currentIterator = this->m_library.Begin();
    Library<CollectableItem*>::Iterator endIterator = this->m_library.End();

    while(currentIterator != endIterator) {
        delete *currentIterator;
        ++currentIterator;
    }
}


void HomeLibrarySystemManager::RunHomeLibrarySystem() {
    HomeLibrarySystemUIHandler uiHandler;
    uiHandler.ShowHomeLibrarySystemWelcomeMessage();
    bool isQuitRequired = false;

    // Main application loop
    while(!isQuitRequired) {
        uiHandler.ShowHomeLibrarySystemMenu();
        HomeLibrarySystemUIHandler::MenuOptions option = uiHandler.GetValidUserOption();

        if(option == HomeLibrarySystemUIHandler::EXIT) {
            isQuitRequired = true;
            uiHandler.ShowHomeLibrarySystemByeByeMessage();
            break;
        }

        bool isCorrectHandlingResult = this->HandleHomeLibrarySystemUserChoice(option);

        if(!isCorrectHandlingResult) {
            uiHandler.ShowHomeLibraryErrorMessage(NULL);
        }
    }
}


void HomeLibrarySystemManager::AddNewItemToLibrary(const std::string& a_itemType) {
    HomeLibrarySystemUIHandler uiHandler;
    std::string itemName;
    std::string itemOwnerName;
    Movie::Genre genre;

    if(a_itemType == "Book") {
        itemName = uiHandler.GetSpecificStringInputFromUser("book's name");
        itemOwnerName = uiHandler.GetSpecificStringInputFromUser("book's author name");
    }
    else { // Movie
        itemName = uiHandler.GetSpecificStringInputFromUser("movie's name");
        itemOwnerName = uiHandler.GetSpecificStringInputFromUser("movie's director name");
        genre = uiHandler.GetMovieGenre();
    }

    long publishYear = uiHandler.GetSpecificNumberInputFromUser("library item's publish year");
    while(publishYear < BASE_COUNTED_YEAR_TIME) {
        uiHandler.ShowHomeLibraryErrorMessage(NULL);
        publishYear = uiHandler.GetSpecificNumberInputFromUser("library item's publish year");
    }

    long collectedYear = uiHandler.GetSpecificNumberInputFromUser("library item's collected year");
    while(collectedYear < publishYear) {
        uiHandler.ShowHomeLibraryErrorMessage(NULL);
        collectedYear = uiHandler.GetSpecificNumberInputFromUser("library item's collected year");
    }

    if(a_itemType == "Book") {
        Book* newBook = new Book(itemName, itemOwnerName, publishYear);
        newBook->SetTimeAddedToCollection(collectedYear);
        this->m_library.Add(newBook);
    }
    else { // Movie
        Movie* newMovie = new Movie(itemName, itemOwnerName, genre, publishYear);
        newMovie->SetTimeAddedToCollection(collectedYear);
        this->m_library.Add(newMovie);
    }

    uiHandler.ShowHomeLibrarySuccessMessage();
}


// void HomeLibrarySystemManager::FetchBooksReleasedXYearsFromNow(const size_t a_yearsFromNow) const {
//     time_t t = time(NULL);
//     struct tm* timeInfo= localtime(&t);

//     size_t startSearchingYear = timeInfo->tm_year  + BASE_COUNTED_YEAR_TIME - a_yearsFromNow;

//     if(startSearchingYear < Book::BOOK_MIN_RELEASED_YEAR) {
//         throw std::invalid_argument("Wrong Input Error - Year must be valid");
//     }

//     for(size_t i = 1; i <= this->m_booksCollection.Size(); ++i) {
//         Book currentBook = this->m_booksCollection.GetBookAt(i);
//         if(startSearchingYear <= currentBook.GetReleasedYear()) {
//             PrintSingleBook(currentBook);
//         }
//     }
// }


// void HomeLibrarySystemManager::FetchMoviesReleasedXYearsFromNow(const size_t a_yearsFromNow) const {
//     time_t t = time(NULL);
//     struct tm* timeInfo= localtime(&t);

//     size_t startSearchingYear = timeInfo->tm_year  + BASE_COUNTED_YEAR_TIME - a_yearsFromNow;

//     if(timeInfo->tm_year + BASE_COUNTED_YEAR_TIME - a_yearsFromNow < Movie::MOVIE_MIN_RELEASED_YEAR) {
//         throw std::invalid_argument("Wrong Input Error - Year must be valid");
//     }

//         for(size_t i = 1; i <= this->m_booksCollection.Size(); ++i) {
//         Movie currentMovie = this->m_moviesCollection.GetMovieAt(i);
//         if(startSearchingYear <= currentMovie.GetReleasedYear()) {
//             PrintSingleMovie(currentMovie);
//         }
//     }
// }


// void HomeLibrarySystemManager::FetchBooksReceivedXYearsFromNow(const size_t a_yearsFromNow) const {
//     time_t t = time(NULL);
//     struct tm* timeInfo= localtime(&t);

//     size_t startSearchingYear = timeInfo->tm_year  + BASE_COUNTED_YEAR_TIME - a_yearsFromNow;

//     if(timeInfo->tm_year + BASE_COUNTED_YEAR_TIME - a_yearsFromNow < Book::BOOK_MIN_RELEASED_YEAR) {
//         throw std::invalid_argument("Wrong Input Error - Year must be valid");
//     }

//         for(size_t i = 1; i <= this->m_booksCollection.Size(); ++i) {
//         Book currentBook = this->m_booksCollection.GetBookAt(i);
//         if(startSearchingYear <= currentBook.GetReceivedYear()) {
//             PrintSingleBook(currentBook);
//         }
//     }
// }


// void HomeLibrarySystemManager::FetchMoviesReceivedXYearsFromNow(const size_t a_yearsFromNow) const {
//     time_t t = time(NULL);
//     struct tm* timeInfo= localtime(&t);

//     if(timeInfo->tm_year + BASE_COUNTED_YEAR_TIME - a_yearsFromNow < Movie::MOVIE_MIN_RELEASED_YEAR) {
//         throw std::invalid_argument("Wrong Input Error - Year must be valid");
//     }

//     size_t startSearchingYear = timeInfo->tm_year  + BASE_COUNTED_YEAR_TIME - a_yearsFromNow;

//     if(timeInfo->tm_year + BASE_COUNTED_YEAR_TIME - a_yearsFromNow < Movie::MOVIE_MIN_RELEASED_YEAR) {
//         throw std::invalid_argument("Wrong Input Error - Year must be valid");
//     }

//         for(size_t i = 1; i <= this->m_moviesCollection.Size(); ++i) {
//         Movie currentMovie = this->m_moviesCollection.GetMovieAt(i);
//         if(startSearchingYear <= currentMovie.GetReceivedYear()) {
//             PrintSingleMovie(currentMovie);
//         }
//     }
// }


// void HomeLibrarySystemManager::FindBooksByMatchingCharactersInName(const char* a_matchingCharacters) const {
//     for(size_t i = 1; i <= this->m_booksCollection.Size(); ++i) {
//         Book currentBook = this->m_booksCollection.GetBookAt(i);
//         if(strstr(currentBook.GetBookName(), a_matchingCharacters)) { // Found the substring in the name
//             PrintSingleBook(currentBook);
//         }
//     }
// }


// void HomeLibrarySystemManager::FindMoviesByMatchingCharactersInName(const char* a_matchingCharacters) const {
//     for(size_t i = 1; i <= this->m_booksCollection.Size(); ++i) {
//         Movie currentMovie = this->m_moviesCollection.GetMovieAt(i);
//         if(strstr(currentMovie.GetMovieName(), a_matchingCharacters)) { // Found the substring in the name
//             PrintSingleMovie(currentMovie);
//         }
//     }
// }