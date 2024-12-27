#include "user_interface.h"

using namespace std;


void UserInterface::addUser(const std::string username) {
    users.insert(username);
}
void UserInterface::printUsers() {
    users.print();
}

void UserInterface::addWatchedMovie(string username, Movie movie) {
    User* user = findUser(username);
    if(user != NULL)
        user->addMovie(movie.getMovieName(), movie);
}
User* UserInterface::findUser(string username) {
    User* user = users.search(username);
    return user;
}
void UserInterface::printWatchedMovies(string username) {
    User* user = findUser(username);
    user->printMovies();
}
BST<Movie>* UserInterface::getWatchedMovies(string username) {
    User* user = findUser(username);
    return user->getMovies();
}
void UserInterface::removeUser(string username) {
    
    User* user = users.search(username);
    if (user != NULL)
        users.remove(username);
}

void UserInterface::removeWatchedMovie(string username, Movie movie) {
    User* user = findUser(username);
    Movie m(movie);
    user->removeMovie(m);
}


void UserInterface::findUsersStartingWith(string prefix, vector<User*> &foundUsers) {
    users.findStartingWith(prefix, foundUsers);

    
}

void UserInterface::findUsersContains(string infix, vector<User*> &foundUsersContains) {
    users.wildcardSearch(infix, foundUsersContains);


}
void UserInterface::findUsersEndingWith(string suffix, vector<User*> &foundUsersEndingWith) {

    users.wildcardSearch(suffix, foundUsersEndingWith);

}
BST<Movie>* UserInterface::mergeWatchedMovies(string username1, string username2) {
    
    User* user1 = findUser(username1);
    User* user2 = findUser(username2);
    
    if(user1==NULL && user2==NULL)
    {
        return NULL;
    }
    
    BST<Movie>* newBST = user1->merge(user2->getMovies());
    return newBST;
}
BST<Movie>* UserInterface::intersectionWatchedMovies(string username1, string username2) {
    User* user1 = findUser(username1);
    User* user2 = findUser(username2);
    BST<Movie>* newBST = user1->intersection(user2->getMovies());
    return newBST;
}

/*





















*/