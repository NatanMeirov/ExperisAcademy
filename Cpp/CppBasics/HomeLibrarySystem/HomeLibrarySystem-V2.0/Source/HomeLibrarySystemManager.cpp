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
#include "../Include/PublishYearInCorrectRangeCriteria.hpp"
#include "../Include/CollectedYearInCorrectRangeCriteria.hpp"
#include "../Include/MatchingSubStringInNameCriteria.hpp"

// Used for inner use
struct DataPrinter {
    enum PrintCriteria {PRINT_BOOK, PRINT_MOVIE};

    DataPrinter(HomeLibrarySystemUIHandler* a_uiHandler, PrintCriteria a_printCriteria)
    : m_uiHandler(a_uiHandler)
    , m_printCriteria(a_printCriteria){
    }

    HomeLibrarySystemUIHandler* m_uiHandler;
    PrintCriteria m_printCriteria;
};


// Concept: Defined PrintItem method of the given T
template <typename T>
void PrintItemUsingUI(const T& a_item, HomeLibrarySystemUIHandler* a_uiHandler) {
    a_uiHandler->PrintItem(a_item);
}


void PrintCollectableItem(CollectableItem& a_item, void* a_dataPrinter) {
    if(a_item.GetCollectableItemType() == "Book") {
        if(static_cast<DataPrinter*>(a_dataPrinter)->m_printCriteria == DataPrinter::PrintCriteria::PRINT_BOOK) {
            PrintItemUsingUI<Book>(static_cast<const Book&>(a_item), static_cast<DataPrinter*>(a_dataPrinter)->m_uiHandler);
        }
        else {
            return; // Only movies should be printed
        }
    }
    else { // Movie
        if(static_cast<DataPrinter*>(a_dataPrinter)->m_printCriteria == DataPrinter::PrintCriteria::PRINT_MOVIE) {
            PrintItemUsingUI<Movie>(static_cast<const Movie&>(a_item), static_cast<DataPrinter*>(a_dataPrinter)->m_uiHandler);
        }
        else {
            return; // Only books should be printed
        }
    }
}


static unsigned int GetMinimumYearSearch(const unsigned int& a_yearsToCheckFromNow, HomeLibrarySystemManager::YearSpecifier a_yearSpecifier) {
    time_t t = time(NULL);
    struct tm* timeInfo= localtime(&t);
    size_t currentYear = timeInfo->tm_year  + HomeLibrarySystemManager::BASE_COUNTED_YEAR_TIME;
    size_t startSearchingYear = currentYear - a_yearsToCheckFromNow;

    if(a_yearSpecifier == HomeLibrarySystemManager::YearSpecifier::COLLECTED_YEAR) {
        if(startSearchingYear < ArtWork::MIN_PUBLISH_YEAR) {
            throw std::invalid_argument("Wrong Year Input Error");
        }
    }
    else { // HomeLibrarySystemManager::YearSpecifier::PUBLISH_YEAR
        if(startSearchingYear < CollectableItem::MIN_COLLECTED_YEAR) {
            throw std::invalid_argument("Wrong Year Input Error");
        }
    }

    return startSearchingYear;
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


bool HomeLibrarySystemManager::HandleHomeLibrarySystemUserChoice(HomeLibrarySystemUIHandler::MenuOptions a_option) {
    HomeLibrarySystemUIHandler uiHandler;

    try {
        // TODO: REMOVE the switch case by using an UIOptions* array[] (each of them will derive from an abstract base class with HandleOption() method and a description() method to generic print them in the menu (open - close principle)) and CALL them like: UIChoices[option]->HandleChoise(...);
        switch(a_option) {
        case HomeLibrarySystemUIHandler::ADD_NEW_BOOK: {
            this->AddNewItemToLibrary("Book");
            break;
        }

        case HomeLibrarySystemUIHandler::ADD_NEW_MOVIE: {
            this->AddNewItemToLibrary("Movie");
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_PUBLISHED_IN_LAST_X_YEARS: {
            long years = uiHandler.GetYearsFromUser();
            unsigned int startSearchingYear = GetMinimumYearSearch(years, HomeLibrarySystemManager::YearSpecifier::PUBLISH_YEAR);
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_BOOK);
            this->TriggerActionOnItemByCriteria(PublishYearInCorrectRangeCriteria(startSearchingYear), PrintCollectableItem, &dataPrinter);
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_PUBLISHED_IN_LAST_X_YEARS: {
            long years = uiHandler.GetYearsFromUser();
            unsigned int startSearchingYear = GetMinimumYearSearch(years, HomeLibrarySystemManager::YearSpecifier::PUBLISH_YEAR);
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_MOVIE);
            this->TriggerActionOnItemByCriteria(PublishYearInCorrectRangeCriteria(startSearchingYear), PrintCollectableItem, &dataPrinter);
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_COLLECTED_IN_LAST_X_YEARS: {
            long years = uiHandler.GetYearsFromUser();
            unsigned int startSearchingYear = GetMinimumYearSearch(years, HomeLibrarySystemManager::YearSpecifier::COLLECTED_YEAR);
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_BOOK);
            this->TriggerActionOnItemByCriteria(CollectedYearInCorrectRangeCriteria(startSearchingYear), PrintCollectableItem, &dataPrinter);
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_COLLECTED_IN_LAST_X_YEARS: {
            long years = uiHandler.GetYearsFromUser();
            unsigned int startSearchingYear = GetMinimumYearSearch(years, HomeLibrarySystemManager::YearSpecifier::COLLECTED_YEAR);
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_MOVIE);
            this->TriggerActionOnItemByCriteria(CollectedYearInCorrectRangeCriteria(startSearchingYear), PrintCollectableItem, &dataPrinter);
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_BOOKS_FOUND_BY_MATCHING_CHARS: {
            std::string subString = uiHandler.GetMatchingCharactersFromUser();
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_BOOK);
            this->TriggerActionOnItemByCriteria(MatchingSubStringInNameCriteria(subString), PrintCollectableItem, &dataPrinter);
            break;
        }

        case HomeLibrarySystemUIHandler::SHOW_ALL_MOVIES_FOUND_BY_MATCHING_CHARS: {
            std::string subString = uiHandler.GetMatchingCharactersFromUser();
            DataPrinter dataPrinter(&uiHandler, DataPrinter::PrintCriteria::PRINT_MOVIE);
            this->TriggerActionOnItemByCriteria(MatchingSubStringInNameCriteria(subString), PrintCollectableItem, &dataPrinter);
            break;
        }

        // Still not supported features:

        // case HomeLibrarySystemUIHandler::FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_BOOKS_TO_X_FOUND_BY_MATCHING_CHARS:
        //     std::string subString = uiHandler.GetMatchingCharactersFromUser();
        //     this->TriggerActionOnItemByPredicate(NULL, (void*)&subString, NULL, NULL); // first NULL - predicate function! second NULL - change collected year to x action function! third - context!
        //     break;

        // case HomeLibrarySystemUIHandler::FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_MOVIES_TO_X_FOUND_BY_MATCHING_CHARS:
        //     std::string subString = uiHandler.GetMatchingCharactersFromUser();
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


void HomeLibrarySystemManager::TriggerActionOnItemByCriteria(const FindItemCriteria& a_findCriteria, ActionCallbackFunction a_action, void* a_contextOfAction) {
    Library<CollectableItem*>::Iterator currentIterator = this->m_library.Begin();
    Library<CollectableItem*>::Iterator endIterator = this->m_library.End();

    while(currentIterator != endIterator) {
        CollectableItem* currentItemPtr = *currentIterator;
        if(a_findCriteria.IsPassingCriteria(*currentItemPtr)) {
            a_action(*currentItemPtr, a_contextOfAction);
        }

        ++currentIterator;
    }
}