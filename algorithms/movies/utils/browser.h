#ifndef MOVIES_BROWSER_H
#define MOVIES_BROWSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../model/movie.h"

class Browser {
private:
	std::string file_name;

public:
	Browser();


	Movie parseMovie(const std::string &line);

	std::vector<std::string> split(const std::string &s, char delimiter);

	std::vector<Movie> loadMovies(int max_size);
};

#endif //MOVIES_BROWSER_H
