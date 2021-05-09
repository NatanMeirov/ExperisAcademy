#ifndef __HOMELIBRARYSYSTEM_HPP__
#define __HOMELIBRARYSYSTEM_HPP__


#include <cstddef> // size_t
#include "BookModules/LinkedListBook.hpp"
#include "MovieModules/LinkedListMovie.hpp"


class HomeLibrarySystemManager {
public:

    enum MenuOptions {
        OPTIONS_START = 0,
        ADD_NEW_BOOK = 1,
        ADD_NEW_MOVIE,
        SHOW_ALL_BOOKS_IN_X_RELEASED_YEARS,
        SHOW_ALL_MOVIES_IN_X_RELEASED_YEARS,
        SHOW_ALL_BOOKS_IN_X_RECEIVED_YEARS,
        SHOW_ALL_MOVIES_IN_X_RECEIVED_YEARS,
        FIND_ALL_BOOKS_BY_MATCHING_CHARS,
        FIND_ALL_MOVIES_BY_MATCHING_CHARS,
        EXIT,
        OPTIONS_END
    };

    HomeLibrarySystemManager();
    HomeLibrarySystemManager(const HomeLibrarySystemManager& a_other);
    HomeLibrarySystemManager& operator=(const HomeLibrarySystemManager& a_other);
    ~HomeLibrarySystemManager();

    // Methods
    void RunHomeLibrarySystem();

private:

    // Methods
    void AddNewBookToLibrary();
    void AddNewMovieToLibrary();

    // Const Methods
    bool HandleHomeLibrarySystemUserChoice(const MenuOptions a_userOption);
    void FetchBooksReleasedXYearsFromNow(const size_t a_yearsFromNow) const;
    void FetchMoviesReleasedXYearsFromNow(const size_t a_yearsFromNow) const;
    void FetchBooksReceivedXYearsFromNow(const size_t a_yearsFromNow) const;
    void FetchMoviesReceivedXYearsFromNow(const size_t a_yearsFromNow) const;
    void FindBooksByMatchingCharactersInName(const char* a_matchingCharacters) const;
    void FindMoviesByMatchingCharactersInName(const char* a_matchingCharacters) const;

    // Members
    LinkedListBook m_booksCollection;
    LinkedListMovie m_moviesCollection;
};


#endif // __HOMELIBRARYSYSTEM_HPP__