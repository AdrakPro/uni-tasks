#include "browser.h"

Browser::Browser() {
	this->file_name = "data/dane.csv";
}

std::vector<std::string> Browser::split(const std::string &s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream token_stream(s);

	while (std::getline(token_stream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<Movie> Browser::loadMovies(int max_size) {
	std::vector<Movie> movies;

	std::ifstream file(
			"data/dane.csv"
	);
	if (!file.is_open()) {
		std::cerr << "Error opening file." << std::endl;
		return movies;
	}

	std::string line;
	int movies_loaded = 0;
	while (std::getline(file, line) && movies_loaded < max_size) {
		std::vector<std::string> parts = split(line, ',');
		if (parts.size() >= 3) {
			int index = std::stoi(parts[0]);
			std::string title;
			int i = 1;

			if (parts[i].front() == '\"') {
				title = parts[++i];

				while (i < parts.size() && parts[i].back() != '\"') {
					title += "," + parts[++i];
				}

				title += "," + parts[i];
			} else {
				title = parts[i];
			}

			if (!title.empty() && title.front() == '\"' && title.back() == '\"') {
				title = title.substr(1, title.size() - 2);
			}

			int rating = 0;

//		Skip invalid format data that have multiple delimiters in title without ""
			try {
				rating = std::stoi(parts[i + 1]);
				movies.emplace_back(index, title, rating);
				++movies_loaded;
			} catch (const std::invalid_argument &e) {
				continue;
			}
		}
	}

	return movies;
}