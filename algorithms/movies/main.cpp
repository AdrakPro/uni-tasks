#include <chrono>
#include <cmath>
#include "utils/browser.h"
#include "model/movie.h"
#include "algorithms/sorting.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Podaj wielkosc struktury!" << std::endl;
		return EXIT_FAILURE;
	}

	int data_size = atoi(argv[1]);

	if (data_size <= 0) {
		std::cout << "Nie poprawna liczba!" << std::endl;
		return EXIT_FAILURE;
	}

	auto* file_browser = new Browser();
	std::vector<Movie> movies = file_browser->loadMovies(data_size);
	int size = static_cast<int>(movies.size());

	auto* movie_manager = new MovieManager();
	movie_manager->setMovies(movies);
	Sorting sorting;

	{
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorting.mergeSort(movies);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Merge sort took " << time << " ms!" << std::endl;
	}

	{
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorting.quickSort(movies, 0, size - 1);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Quick sort took " << time << " ms!" << std::endl;
	}

	{
		int max_depth = 2 * floor(log2(size));
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorting.introSort(movies, max_depth);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Intro sort took " << time << " ms!" << std::endl;
	}

	{
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorting.heapSort(movies);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Heap sort took " << time << " ms!" << std::endl;
	}

	{
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorting.insertionSort(movies);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Insertion sort took " << time << " ms!" << std::endl;
	}

	{
		std::vector<Movie> sorted;
		const int BUCKETS_NUM = 11;
		std::chrono::high_resolution_clock::time_point start, end;

		start = std::chrono::high_resolution_clock::now();
		sorted = sorting.bucketSort(movies, BUCKETS_NUM);
		end = std::chrono::high_resolution_clock::now();

		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start
		).count();

		std::cout << "Bucket sort took " << time << " ms!" << std::endl;


		movie_manager->averageRating();

		movie_manager->medianRating(sorted);
	}
	return EXIT_SUCCESS;
}

