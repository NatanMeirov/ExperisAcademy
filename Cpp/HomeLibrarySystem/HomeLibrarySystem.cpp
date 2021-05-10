#include "HomeLibrarySystem.hpp"
#include <cstddef> // size_t
#include <cstring> // strcpy
#include <exception>
#include <stdexcept> // std::invalid_argument
#include <ctime> // time, localtime
#include "ScreenUIManager.hpp"
#include "BookModules/Book.hpp"
#include "BookModules/LinkedListBook.hpp"
#include "MovieModules/Movie.hpp"
#include "MovieModules/LinkedListMovie.hpp"


#define BUF_SIZE 1024


static void InitializeHomeLibrarySystemMenu(char* a_menu) {
    strcpy(a_menu, "\nPlease enter your choice number:\n\n"
    "[*] Press 1 to Add new Book to the library\n"
    "[*] Press 2 to Add new Movie to the library\n"
    "[*] Press 3 to Show all Books that released in the last X years\n"
    "[*] Press 4 to Show all Movies that released in the last X years\n"
    "[*] Press 5 to Show all Books that received in the last X years\n"
    "[*] Press 6 to Show all Movies that received in the last X years\n"
    "[*] Press 7 to Find all Books with matching characters in their name\n"
    "[*] Press 8 to Find all Movies with matching characters in their name\n"
    "[*] Press 9 to Exit the HomeLibrarySystem");
}


static long GetYearsFromUser() {
    ScreenUIManager ui;
    ui.ShowMessage("Enter how many years from now:", true);

    bool isCorrectInput = false;
    long userChoice;
    while(!isCorrectInput) {
        try {
            ui.GetInputAsNumber(userChoice);
            isCorrectInput = true; // Get here if input is correct
        }
        catch(const std::exception& ex) {
            ui.ShowMessage(ex.what(), true);
            ui.ShowMessage("Please try again:", true);
            isCorrectInput = false;
        }
    }

    if(userChoice < 0) {
        userChoice *= -1; // Absolute value
    }

    return userChoice;
}


static void GetMatchingCharactersFromUser(char* a_buffer, size_t a_bufferLimit) {
    ScreenUIManager ui;
    ui.ShowMessage("Enter matching characters to search:", true);
    ui.GetInputAsString(a_buffer, a_bufferLimit);
}


static long GetMovieGenre() {
    ScreenUIManager ui;
    ui.ShowMessage("Please enter the Movie's genre:", true);
    ui.ShowMessage("Enter one of the options: 1 for Action, 2 for Comedy, 3 for Drama, 4 for Anime, or 5 for Other Genre", true);
    long userInput;
    ui.GetInputAsNumber(userInput);

    return userInput;
}


static void PrintSingleBook(const Book& a_book) {

}


static void PrintSingleMovie(const Movie& a_movie) {
    
}



bool HomeLibrarySystemManager::HandleHomeLibrarySystemUserChoice(HomeLibrarySystemManager::MenuOptions a_userOption) {
    char buffer[BUF_SIZE];
    ScreenUIManager ui;

    try {
        switch(a_userOption) {
        case HomeLibrarySystemManager::ADD_NEW_BOOK:
            this->AddNewBookToLibrary();
            break;

        case HomeLibrarySystemManager::ADD_NEW_MOVIE:
            this->AddNewMovieToLibrary();
            break;

        case HomeLibrarySystemManager::SHOW_ALL_BOOKS_IN_X_RELEASED_YEARS:
            this->FetchBooksReleasedXYearsFromNow(GetYearsFromUser());
            break;

        case HomeLibrarySystemManager::SHOW_ALL_MOVIES_IN_X_RELEASED_YEARS:
            this->FetchMoviesReleasedXYearsFromNow(GetYearsFromUser());
            break;

        case HomeLibrarySystemManager::SHOW_ALL_BOOKS_IN_X_RECEIVED_YEARS:
            this->FetchBooksReceivedXYearsFromNow(GetYearsFromUser());
            break;

        case HomeLibrarySystemManager::SHOW_ALL_MOVIES_IN_X_RECEIVED_YEARS:
            this->FetchMoviesReceivedXYearsFromNow(GetYearsFromUser());
            break;

        case HomeLibrarySystemManager::FIND_ALL_BOOKS_BY_MATCHING_CHARS:
            GetMatchingCharactersFromUser(buffer, BUF_SIZE);
            this->FindBooksByMatchingCharactersInName(buffer);

        case HomeLibrarySystemManager::FIND_ALL_MOVIES_BY_MATCHING_CHARS:
            GetMatchingCharactersFromUser(buffer, BUF_SIZE);
            this->FindMoviesByMatchingCharactersInName(buffer);

        default:
            return false;
        }
    }
    catch(const std::exception& ex) {
        ui.ShowMessage(ex.what(), true);
        return false;
    }

    return true;
}


HomeLibrarySystemManager::HomeLibrarySystemManager()
: m_booksCollection()
, m_moviesCollection() {
}


HomeLibrarySystemManager::HomeLibrarySystemManager(const HomeLibrarySystemManager& a_other)
: m_booksCollection(a_other.m_booksCollection)
, m_moviesCollection(a_other.m_moviesCollection) {
}

HomeLibrarySystemManager& HomeLibrarySystemManager::operator=(const HomeLibrarySystemManager& a_other) {
    if(this == &a_other) {
        return *this;
    }

    this->m_booksCollection = a_other.m_booksCollection;
    this->m_moviesCollection = a_other.m_moviesCollection;

    return *this;
}


HomeLibrarySystemManager::~HomeLibrarySystemManager() {
}


void HomeLibrarySystemManager::RunHomeLibrarySystem() {// Talk to the ScreenUIManager from this method
    ScreenUIManager ui;
    bool isCorrectChoice = false, isQuitRequired = false;
    char menuMessage[4096];
    InitializeHomeLibrarySystemMenu(menuMessage);

    ui.ShowMessage("Welcome to HomeLibrarySystem!", true);

    // Main application loop
    while(!isQuitRequired) {
        ui.ShowMenuMessage(menuMessage, true);
        ui.ShowMessage("Enter your choice:", true);

        MenuOptions mappedChoice;
        long userChoice;
        while(!isCorrectChoice) {
            try {
                ui.GetInputAsNumber(userChoice);
                mappedChoice = (MenuOptions)userChoice;
                if(mappedChoice <= HomeLibrarySystemManager::OPTIONS_START || mappedChoice >= HomeLibrarySystemManager::OPTIONS_END) {
                    throw std::invalid_argument("Wrong Input Error");
                }

                isCorrectChoice = true; // Get here if input is correct
            }
            catch(const std::exception& ex) {
                ui.ShowMessage(ex.what(), true);
                ui.ShowMessage("Please try again:", true);
                isCorrectChoice = false;
            }
        }

        if(mappedChoice == HomeLibrarySystemManager::EXIT) {
            isQuitRequired = true;
            ui.ShowMessage("Bye Bye!", true);
            break;
        }

        bool handlingResult = HandleHomeLibrarySystemUserChoice(mappedChoice);

        if(!handlingResult) {
            ui.ShowMessage("An error has occurred, please try again (optional error: invalid input)...", true);
        }

        isCorrectChoice = false;
    }
}


void HomeLibrarySystemManager::AddNewBookToLibrary() {
    ScreenUIManager ui;
    char bookName[BUF_SIZE];
    char authorName[BUF_SIZE];

    ui.ShowMessage("Please enter Book name (max: 1023 characters):", true);
    ui.GetInputAsString(bookName, BUF_SIZE);

    ui.ShowMessage("Please enter Author name (max: 1023 characters):", true);
    ui.GetInputAsString(authorName, BUF_SIZE);

    ui.ShowMessage("Please enter the Book's Released Year (Not before 1900):", true);
    long releasedYear;
    ui.GetInputAsNumber(releasedYear);

    ui.ShowMessage("Please enter the Book's Received Year (Not before 1900):", true);
    long receivedYear;
    ui.GetInputAsNumber(receivedYear);

    Book newBook(bookName, authorName, releasedYear, receivedYear);
    this->m_booksCollection.PushTail(newBook);

    ui.ShowMessage("Successfully added a new Book!", true);
}


void HomeLibrarySystemManager::AddNewMovieToLibrary() {
    ScreenUIManager ui;
    char movieName[BUF_SIZE];
    char directorName[BUF_SIZE];

    ui.ShowMessage("Please enter Movie name (max: 1023 characters):", true);
    ui.GetInputAsString(movieName, BUF_SIZE);

    ui.ShowMessage("Please enter Director name (max: 1023 characters):", true);
    ui.GetInputAsString(directorName, BUF_SIZE);

    Movie::Genre movieGenre = (Movie::Genre)GetMovieGenre();

    ui.ShowMessage("Please enter the Movie's Released Year:", true);
    long releasedYear;
    ui.GetInputAsNumber(releasedYear);

    ui.ShowMessage("Please enter the Movie's Received Year:", true);
    long receivedYear;
    ui.GetInputAsNumber(receivedYear);

    Movie newMovie(movieName, directorName, movieGenre, releasedYear, receivedYear);
    this->m_moviesCollection.PushTail(newMovie);

    ui.ShowMessage("Successfully added a new Movie!", true);
}


void HomeLibrarySystemManager::FetchBooksReleasedXYearsFromNow(const size_t a_yearsFromNow) const {
    time_t t = time(NULL);
    struct tm* timeInfo= localtime(&t);

    if(timeInfo->tm_year - a_yearsFromNow < Book::BOOK_MIN_RELEASED_YEAR) {
        throw std::invalid_argument("Wrong Input Error - Year must be valid");
    }

    // TODO
}


void HomeLibrarySystemManager::FetchMoviesReleasedXYearsFromNow(const size_t a_yearsFromNow) const {
    time_t t = time(NULL);
    struct tm* timeInfo= localtime(&t);

    if(timeInfo->tm_year - a_yearsFromNow < Book::BOOK_MIN_RELEASED_YEAR) {
        throw std::invalid_argument("Wrong Input Error - Year must be valid");
    }

    // TODO
}


void HomeLibrarySystemManager::FetchBooksReceivedXYearsFromNow(const size_t a_yearsFromNow) const {
    time_t t = time(NULL);
    struct tm* timeInfo= localtime(&t);

    if(timeInfo->tm_year - a_yearsFromNow < Book::BOOK_MIN_RELEASED_YEAR) {
        throw std::invalid_argument("Wrong Input Error - Year must be valid");
    }

    // TODO
}


void HomeLibrarySystemManager::FetchMoviesReceivedXYearsFromNow(const size_t a_yearsFromNow) const {
    time_t t = time(NULL);
    struct tm* timeInfo= localtime(&t);

    if(timeInfo->tm_year - a_yearsFromNow < Book::BOOK_MIN_RELEASED_YEAR) {
        throw std::invalid_argument("Wrong Input Error - Year must be valid");
    }

    // TODO
}


void HomeLibrarySystemManager::FindBooksByMatchingCharactersInName(const char* a_matchingCharacters) const {
    // TODO
}


void HomeLibrarySystemManager::FindMoviesByMatchingCharactersInName(const char* a_matchingCharacters) const {
    // TODO
}