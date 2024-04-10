#include "movie.h"

#include <utility>

Movie::Movie(int id, std::string title, int rating) {
	this->id = id;
	this->title = std::move(title);
	this->rating = rating;
}

Movie::Movie() {
	this->id = 0;
	this->title = nullptr;
	this->rating = 0;
}

void MovieManager::addMovie(const Movie &movie) {
	this->movies.push_back(movie);
}

void MovieManager::displayMovies() const {
	for (const auto &movie: movies) {
		std::cout << "ID: " << movie.id << ", Title: " << movie.title << ", Rating: "
							<< movie.rating << std::endl;
	}
}

void MovieManager::averageRating() const {
	if (movies.empty()) {
		return;
	}

	int result = 0;

	for (const auto &movie: movies) {
		result += movie.rating;
	}


	std::cout << "Avg: " << result / static_cast<int>(movies.size()) << std::endl;
}

void MovieManager::medianRating(const std::vector<Movie> &sorted) {
	int size = static_cast<int>(movies.size());
	int mid = size / 2;
	int result = 0;

	if (size == 0) {
		return;
	}

	if (size % 2 == 0) {
		result = (sorted[mid - 1].rating + sorted[mid].rating) / 2;
	} else {
		result = sorted[mid].rating;
	}

	std::cout << "Median: " << result << std::endl;
}

void MovieManager::setMovies(std::vector<Movie> &data) {
	this->movies = data;
}

int MovieManager::getSize() const {
	return static_cast<int>(movies.size());
}


// W celu przetesowania kodu, trzeba usunać komentarz i wykonać polecenie:
// g++ -o test movie.cpp -std=c++17 && ./test
//
//bool isSorted(const std::vector<Movie> &data) {
//	for (size_t i = 1; i < data.size(); ++i) {
//		if (data[i - 1].rating > data[i].rating) {
//			return false;
//		}
//	}
//
//	return true;
//}
//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../tests/catch.hpp"
//
//TEST_CASE("Sorting movies by title in ascending order") {
//	MovieManager manager;
//
//	manager.addMovie(Movie(0, "Supermen", 5.8));
//	manager.addMovie(Movie(1, "Batman", 8.6));
//	manager.addMovie(Movie(2, "Chlopiec w pasiastej pizamie", 10.0));
//	manager.addMovie(Movie(3, "Kobiety Mafii", 2.0));
//	manager.addMovie(Movie(4, "Szeregowiec Rayan", 8.7));
//
//	std::vector<Movie> movie_copy = manager.getMovies();
//
//	SECTION("Merge sort") {
//		std::vector<Movie> sorted = manager.mergeSort(movie_copy);
//		REQUIRE(isSorted(sorted));
//	}
//
//	SECTION("Quick sort") {
//		int left = 0;
//		int right = manager.getSize() - 1;
//		std::vector<Movie> sorted = manager.quickSort(movie_copy, left, right);
//		REQUIRE(isSorted(sorted));
//	}
//
//	SECTION("Bucket sort") {
//		std::vector<Movie> sorted = manager.bucketSort(movie_copy, 10);
//		REQUIRE(isSorted(sorted));
//	}
//
//	SECTION("Insertion sort") {
//		std::vector<Movie> sorted = manager.insertionSort(movie_copy);
//		REQUIRE(isSorted(sorted));
//	}
//
//	SECTION("Heap sort") {
//		std::vector<Movie> sorted = manager.heapSort(movie_copy);
//		REQUIRE(isSorted(sorted));
//	}
//
//	SECTION("Intro sort") {
//		int depth_max = 2*floor(log2(manager.getSize()));
//		std::vector<Movie> sorted = manager.introSort(movie_copy, depth_max);
//		REQUIRE(isSorted(sorted));
//	}
//}