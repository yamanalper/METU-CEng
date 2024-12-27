#ifndef USER_H
#define USER_H

#include <string>
#include "bst.h"
#include "movie.h"

class User {
private:
    std::string username;
    BST<Movie> movies;
public:
    User() : username(""), movies() {}
    User(std::string username) : username(username),  movies() {}
    std::string getUsername() { return username; }
    BST<Movie>* getMovies() { return &movies; }
    void addMovie(std::string movieName, Movie movie) { movies.insert(movieName,movie); }
    void removeMovie(Movie movie) { movies.remove(movie.getMovieName()); }
    void searchMovie(Movie movie) { movies.search(movie.getMovieName()); }
    void printMovies() { movies.print(); }
    BST<Movie>* merge(BST<Movie>* bst) { return movies.merge(bst); }
    BST<Movie>* intersection(BST<Movie>* bst) { return movies.intersection(bst); }
    friend std::ostream& operator<<(std::ostream& os, const User& user) {
    os << user.username;
    return os;
}
};


#endif // USER_H
