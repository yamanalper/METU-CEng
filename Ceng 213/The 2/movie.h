#ifndef MOVIE_H
#define MOVIE_H

#include <string>
class Movie {
private:
    
    std::string movieName;
    int year;
    float rating;

public:// do not change.
    Movie() : movieName(""), year(0), rating(0) {}
    
    Movie(std::string movieName, int year, float rating) : movieName(movieName), year(year), rating(rating) {}
    Movie(const Movie& movie) : movieName(movie.getMovieName()), year(movie.getYear()), rating(movie.getRating()) {}
    std::string getMovieName() const { return movieName; }
    int getYear() const { return year; }
    float getRating() const { return rating; }
    bool operator==(const Movie &obj){
        return movieName == obj.movieName && year == obj.year && rating==obj.rating;
    }
};

#endif // MOVIE_H
