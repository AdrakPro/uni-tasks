#ifndef MOVIES_MOVIE_H
#define MOVIES_MOVIE_H

#include <string>
#include <vector>
#include <iostream>

class Movie {
public:
	int id;
	std::string title;
	int rating;

	Movie(int id, std::string title, int rating);

	Movie();
};

class MovieManager {
private:
	std::vector<Movie> movies;

public:
	MovieManager() = default;

	void addMovie(const Movie &movie);

	void displayMovies() const;

	[[nodiscard]] int getSize() const;

	void averageRating() const;

	void medianRating(const std::vector<Movie> &sorted);

	void setMovies(std::vector<Movie> &data);
};

#endif //MOVIES_MOVIE_H
