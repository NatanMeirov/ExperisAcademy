#include "../MovieModules/Movie.hpp"
#include <iostream> // std::cout, std::endl


int main() {
    Movie movie("MyMovie", "Me", Movie::COMEDY, 2021, "07/05/2021");

    std::cout << movie.GetMovieName() << std::endl;
    std::cout << movie.GetDirector() << std::endl;
    std::cout << movie.GetGenre() << std::endl;
    std::cout << movie.GetGenreAsString() << std::endl;
    std::cout << movie.GetReleaseYear() << std::endl;
    std::cout << movie.GetReceivedDate() << std::endl;

    return 0;
}