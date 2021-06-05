#ifndef __HOMELIBRARYSYSTEMUIHANDLER_HPP__
#define __HOMELIBRARYSYSTEMUIHANDLER_HPP__


#include <exception> // std::exception
#include <string> // std::string
#include "ScreenUIManager.hpp"
#include "Book.hpp"
#include "Movie.hpp"


// A UI Handler layer to handle the Application's Users' inputs
class HomeLibrarySystemUIHandler : private ScreenUIManager {
public:

    enum MenuOptions {
        OPTIONS_START = 0,
        ADD_NEW_BOOK = 1,
        ADD_NEW_MOVIE,
        SHOW_ALL_BOOKS_PUBLISHED_IN_LAST_X_YEARS,
        SHOW_ALL_MOVIES_PUBLISHED_IN_LAST_X_YEARS,
        SHOW_ALL_BOOKS_COLLECTED_IN_LAST_X_YEARS,
        SHOW_ALL_MOVIES_COLLECTED_IN_LAST_X_YEARS,
        SHOW_ALL_BOOKS_FOUND_BY_MATCHING_CHARS,
        SHOW_ALL_MOVIES_FOUND_BY_MATCHING_CHARS,
        FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_BOOKS_TO_X_FOUND_BY_MATCHING_CHARS,
        FIND_AND_CHANGE_COLLECTED_YEAR_OF_ALL_MOVIES_TO_X_FOUND_BY_MATCHING_CHARS,
        EXIT,
        OPTIONS_END
    };


    void ShowHomeLibrarySystemWelcomeMessage() const;
    void ShowHomeLibrarySystemByeByeMessage() const;
    void ShowHomeLibraryErrorMessage(const char* a_errorMsg) const; // Optional message (can be NULL)
    void ShowHomeLibrarySystemMenu() const;
    void ShowHomeLibrarySuccessMessage() const;
    MenuOptions GetValidUserOption() const;
    long GetYearsFromUser() const;
    Movie::Genre GetMovieGenre() const;
    std::string GetMatchingCharactersFromUser() const;
    std::string GetSpecificStringInputFromUser(const char* a_specificInputRequestTitle) const;
    long GetSpecificNumberInputFromUser(const char* a_specificInputRequestTitle) const;
    void PrintItem(const Book& a_book) const;
    void PrintItem(const Movie& a_movie) const;
};


#endif // __HOMELIBRARYSYSTEMUI_HPP__