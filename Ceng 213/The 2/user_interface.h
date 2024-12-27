
#include "trie.h"
#include <string>
#include <vector>
#include"movie.h"
#include "user.h"

class UserInterface {
public:

    void addUser(std::string username);
    void removeUser(std::string username);
    void addWatchedMovie(std::string username, Movie movie);
    void removeWatchedMovie(std::string username, Movie movie);
    User* findUser(std::string username);    
    BST<Movie>* getWatchedMovies(std::string username);
    BST<Movie>* mergeWatchedMovies(std::string username1, std::string username2);
    BST<Movie>* intersectionWatchedMovies(std::string username1, std::string username2);

    void findUsersStartingWith(std::string prefix, std::vector<User*>& foundUsers);
    void findUsersContains(std::string infix, vector<User*> &foundUsersContains);
    void findUsersEndingWith(std::string suffix, vector<User*> &foundUsersEndingWith);

    void printUsers();
    void printWatchedMovies(std::string username);

    
    
private:
    Trie<User> users;
};
