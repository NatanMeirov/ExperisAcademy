#include "../Include/HomeLibrarySystemUIHandler.hpp"
#include <exception> // std::exception
#include <stdexcept> // std::invalid_argument
#include <string> // std::string


void HomeLibrarySystemUIHandler::ShowHomeLibrarySystemWelcomeMessage() const {
    this->ShowMessage("Welcome to HomeLibrarySystem!", true);
}


void HomeLibrarySystemUIHandler::ShowHomeLibrarySystemByeByeMessage() const {
    this->ShowMessage("Bye Bye!", true);
}


void HomeLibrarySystemUIHandler::ShowHomeLibraryErrorMessage(const char* a_errorMsg) const {
    this->ShowMessage("An error has occurred, please try again (optional error: invalid input)...", true);
    if(a_errorMsg) {
        this->ShowMessage(a_errorMsg, true);
    }
}


void HomeLibrarySystemUIHandler::ShowHomeLibrarySystemMenu() const {
    const char* menuMessage = "\nPlease enter your choice number:\n\n"
    "[*] Press 1 to Add new Book to the library\n"
    "[*] Press 2 to Add new Movie to the library\n"
    "[*] Press 3 to Show all Books published in the last {X} years\n"
    "[*] Press 4 to Show all Movies that published in the last {X} years\n"
    "[*] Press 5 to Show all Books that collected in the last {X} years\n"
    "[*] Press 6 to Show all Movies that collected in the last {X} years\n"
    "[*] Press 7 to Show all Books found by matching characters in their names\n"
    "[*] Press 8 to Find all Movies found by matching characters in their names\n"
    "[*] Press 9 to Find and Change the collected year of all Books to {X} found by matching characters in their names\n"
    "[*] Press 10 to Find and Change the collected year of all Movies to {X} found by matching characters in their names\n"
    "[*] Press 11 to Exit the HomeLibrarySystem";

    this->ShowMenuMessage(menuMessage, true);
}


void HomeLibrarySystemUIHandler::ShowHomeLibrarySuccessMessage() const {
    this->ShowMessage("Successfully done!", true);
}


HomeLibrarySystemUIHandler::MenuOptions HomeLibrarySystemUIHandler::GetValidUserOption() const {
    bool isCorrectChoice = false;
    long userChoice;
    MenuOptions mappedChoice;
    while(!isCorrectChoice) {
        try {
            this->ShowMessage("Your choice: ", false);
            this->GetInputAsNumber(userChoice);
            mappedChoice = (MenuOptions)userChoice;
            if(mappedChoice <= HomeLibrarySystemUIHandler::OPTIONS_START || mappedChoice >= HomeLibrarySystemUIHandler::OPTIONS_END) {
                throw std::invalid_argument("Wrong Input Error");
            }

            // Gets here if input is correct
            isCorrectChoice = true;
        }
        catch(const std::exception& ex) {
            this->ShowMessage(ex.what(), true);
            this->ShowMessage("Please try again:", true);
            isCorrectChoice = false;
        }
    }

    return mappedChoice;
}


long HomeLibrarySystemUIHandler::GetYearsFromUser() const {
    bool isCorrectInput = false;
    long userChoice;
    while(!isCorrectInput) {
        try {
            this->ShowMessage("Enter how many years from now: ", false);
            this->GetInputAsNumber(userChoice);
            isCorrectInput = true; // Get here if input is correct
        }
        catch(const std::exception& ex) {
            this->ShowMessage(ex.what(), true);
            this->ShowMessage("Please try again...", true);
            isCorrectInput = false;
        }
    }

    if(userChoice < 0) {
        userChoice *= -1; // Absolute value
    }

    return userChoice;
}


std::string HomeLibrarySystemUIHandler::GetMatchingCharactersFromUser() const {
    this->ShowMessage("Enter matching characters to search: ", false);

    char buffer[this->MAX_BUFFER_LENGTH];
    this->GetInputAsString(buffer, this->MAX_BUFFER_LENGTH);

    return std::string(buffer);
}


Movie::Genre HomeLibrarySystemUIHandler::GetMovieGenre() const {
    this->ShowMessage("Please enter the Movie's genre:", true);
    this->ShowMessage("(Enter one of the options: 1 for Action, 2 for Comedy, 3 for Drama, 4 for Anime, or 5 for Other Genre)", true);
    long userInput;
    this->GetInputAsNumber(userInput);

    return Movie::Genre(userInput);
}


std::string HomeLibrarySystemUIHandler::GetSpecificStringInputFromUser(const char* a_specificInputRequestTitle) const {
    this->ShowMessage("Please enter ", false);
    this->ShowMessage(a_specificInputRequestTitle, false);
    this->ShowMessage(": ", false);

    char buffer[this->MAX_BUFFER_LENGTH];
    this->GetInputAsString(buffer, this->MAX_BUFFER_LENGTH);

    return std::string(buffer);
}


long HomeLibrarySystemUIHandler::GetSpecificNumberInputFromUser(const char* a_specificInputRequestTitle) const {
    bool isCorrectInput = false;
    long userChoice;
    while(!isCorrectInput) {
        try {
            this->ShowMessage("Please enter ", false);
            this->ShowMessage(a_specificInputRequestTitle, false);
            this->ShowMessage(" :", false);
            this->GetInputAsNumber(userChoice);
            isCorrectInput = true; // Get here if input is correct
        }
        catch(const std::exception& ex) {
            this->ShowMessage(ex.what(), true);
            this->ShowMessage("Please try again...", true);
            isCorrectInput = false;
        }
    }

    return userChoice;
}


void HomeLibrarySystemUIHandler::PrintItem(const Book& a_book) const {
    this->ShowMessage("*) Book: ", false);
    this->ShowMessage(a_book.GetArtWorkName().c_str(), false);
    this->ShowMessage(", By: ", false);
    this->ShowMessage(a_book.GetPublisherName().c_str(), false);
    this->ShowMessage(".", true);
}


void HomeLibrarySystemUIHandler::PrintItem(const Movie& a_movie) const {
    this->ShowMessage("*) Movie: ", false);
    this->ShowMessage(a_movie.GetArtWorkName().c_str(), false);
    this->ShowMessage(", By: ", false);
    this->ShowMessage(a_movie.GetPublisherName().c_str(), false);
    this->ShowMessage(".", true);
}