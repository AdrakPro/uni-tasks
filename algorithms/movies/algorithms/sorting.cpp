#include "sorting.h"

std::vector<Movie>
Sorting::merge(std::vector<Movie> left, std::vector<Movie> right) {
	std::vector<Movie> result;
	int size = static_cast<int>(left.size() + right.size());
	int i = 0;
	int j = 0;

	for (int k = 0; k < size; ++k) {
		if (left.size() > i && right.size() > j) {
			if (left[i].rating <= right[j].rating) {
				result.push_back(left[i]);
				++i;
			} else {
				result.push_back(right[j]);
				++j;
			}
		} else if (left.size() <= i) {
			result.push_back(right[j]);
			++j;
		} else if (right.size() <= j) {
			result.push_back(left[i]);
			++i;
		}
	}

	return result;
}

// Avg O(n log n) Worst O(n log n)
std::vector<Movie> Sorting::mergeSort(std::vector<Movie> &data) {
	int size = static_cast<int>(data.size()); // O(1)

	if (size <= 1) { // O(1)
		return data; // O(1)
	}

	int mid = size / 2; // O(1)

	std::vector<Movie> left_half(data.begin(), data.begin() + mid); // O(n)
	std::vector<Movie> right_half(data.begin() + mid, data.end()); // O(n)

	std::vector<Movie> left = mergeSort(left_half); // O(log n)
	std::vector<Movie> right = mergeSort(right_half); // O(log n)

	return merge(left, right); // O(n)
}

void Sorting::swap(std::vector<Movie> &data, int i1, int i2) {
	if (i1 != i2) {
		Movie aux = data[i1];
		data[i1] = data[i2];
		data[i2] = aux;
	}
}

int Sorting::medianOfThree(std::vector<Movie> &data, int low, int high) {
	int mid = low + (high - low) / 2;

	if (data[high].rating < data[low].rating) {
		swap(data, low, high);
	}

	if (data[mid].rating < data[low].rating) {
		swap(data, low, mid);
	}

	if (data[high].rating < data[mid].rating) {
		swap(data, mid, high);
	}

	return mid;
}

std::vector<Movie> Sorting::quickSort(std::vector<Movie> &data, int low, int high) {
	if (high <= low + 16 - 1) {
		insertionSort(data, low, high);
		return data;
	}

	int partition_index = partition(data, low, high);
	quickSort(data, low, partition_index - 1);
	quickSort(data, partition_index + 1, high);
	std::cout << "Huj" << std::endl;
	return data;
}

int Sorting::partition(std::vector<Movie> &data, int low, int high) {
	Movie pivot = data[medianOfThree(data, low, high)];
	int i = low - 1;

	for (int j = low; j < high; ++j) {
		if (data[j].rating < pivot.rating) {
			++i;
			swap(data, i, j);
		}
	}

	swap(data, i + 1, high);
	return i + 1;
}

// std::list.sort is O(n log n) https://cplusplus.com/reference/list/list/sort/
void Sorting::nextSort(std::list<Movie> &bucket) {
	bucket.sort(
			[](const Movie &a, const Movie &b) {
				return a.rating <= b.rating;
			}
	);
}

// Avg O(n log m) Worst O(n^2)
std::vector<Movie> Sorting::bucketSort(std::vector<Movie> &data, int buckets_num) {
	std::vector<std::list<Movie>> buckets(buckets_num); // O(k)
	int max_rating = 0; // (O(1)

	for (const Movie &movie: data) { // O(n)
		if (movie.rating > max_rating) { // O(1)
			max_rating = movie.rating; // O(1)
		}
	}

	for (const Movie &movie: data) { // O(n)
		int index = static_cast<int>(buckets_num * movie.rating / (max_rating + 1));  // O(1)
		buckets[index].push_back(movie); // O(1) amort.
	}

	for (std::list<Movie> &bucket: buckets) { // O(k)
		// Used std comparator sort, docs said the time complexity is:
		nextSort(bucket); // Avg O(m log m) Worst O(m^2)
	}

	std::vector<Movie> sorted; // O(1)
	for (const std::list<Movie> &bucket: buckets) { // O(k)
		sorted.insert(sorted.end(), bucket.begin(), bucket.end()); // O(m)
	}

	return sorted; // O(1)
}

void Sorting::heapify(std::vector<Movie> &data, int size, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < size && data[left].rating > data[largest].rating) {
		largest = left;
	}

	if (right < size && data[right].rating > data[largest].rating) {
		largest = right;
	}

	if (largest != i) {
		std::swap(data[i], data[largest]);
		heapify(data, size, largest);
	}
}

std::vector<Movie> Sorting::heapSort(std::vector<Movie> &data) {
	int size = static_cast<int>(data.size());
	int mid = size / 2;

	for (int i = mid - 1; i >= 0; --i) {
		heapify(data, size, i);
	}

	for (int i = size - 1; i > 0; --i) {
		std::swap(data[0], data[i]);
		heapify(data, i, 0);
	}

	return data; // O(1)
}

std::vector<Movie> Sorting::insertionSort(std::vector<Movie> &data, int low, int high) {
	for (int i = high; i > low; i--) {
		if (data[i].rating < data[i - 1].rating) swap(data, i - 1, i);
	}

	for (int i = low + 2; i <= high; i++) {
		int j = i;
		Movie v = data[i];
		while (v.rating < data[j - 1].rating) {
			data[j] = data[j - 1];
			j--;
		}
		data[j] = v;
	}

	return data; // O(1)
}

// Avg O(n log n) Worst O(n^2)
std::vector<Movie> Sorting::introSort(std::vector<Movie> &data, int max_depth) {
	int size = static_cast<int>(data.size()); // O(1)
	int low = 0; // O(1)
	int high = size - 1; // O(1)

	if (size <= 16) { // O(1)
		insertionSort(data, low, high); // O(n^2)
	} else if (max_depth == 0) { // O(1)
		heapSort(data); // O(n log n)
	} else {
		// Used pivot as median of three
		int partition_index = partition(data, low, high); // O(n)

		std::vector<Movie> left_partition(data.begin(), data.begin() + partition_index); // O(n)
		std::vector<Movie> right_partition(data.begin() + partition_index + 1, data.end()); // O(n)

		introSort(left_partition, max_depth - 1); // O(log n)
		introSort(right_partition, max_depth - 1); // O(log n)
	}

	return data; // O(1)
}


// Aby przetestować kod trzeba udkomentować oraz wykonać poniższe polecenie:
// g++ -o test sorting.cpp ../model/movie.cpp -std=c++17 && ./test
//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../tests/catch.hpp"
//bool Sorting::isSorted(const std::vector<Movie> &data) {
//	for (size_t i = 1; i < data.size(); ++i) {
//		if (data[i - 1].rating > data[i].rating) {
//			return false;
//		}
//	}
//
//	return true;
//}
//
//TEST_CASE("Sorting movies by title in ascending order") {
//	std::vector<Movie> movies = {
//			Movie(0, "Supermen", 5),
//			Movie(1, "Batman", 8),
//			Movie(2, "Chlopiec w pasiastej pizamie", 10),
//			Movie(3, "Kobiety Mafii", 2),
//			Movie(4, "Szeregowiec Rayan", 8),
//	};
//
//	Sorting sorting;
//
//	SECTION("Merge sort") {
//		std::vector<Movie> sorted = sorting.mergeSort(movies);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//
//	SECTION("Quick sort") {
//		int left = 0;
//		int right = movies.size() - 1;
//		std::vector<Movie> sorted = sorting.quickSort(movies, left, right);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//
//	SECTION("Bucket sort") {
//		std::vector<Movie> sorted = sorting.bucketSort(movies, 10);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//
//	SECTION("Insertion sort") {
//		std::vector<Movie> sorted = sorting.insertionSort(movies);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//
//	SECTION("Heap sort") {
//		std::vector<Movie> sorted = sorting.heapSort(movies);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//
//	SECTION("Intro sort") {
//		int depth_max = 2 * floor(log2(movies.size()));
//		std::vector<Movie> sorted = sorting.introSort(movies, depth_max);
//		REQUIRE(sorting.isSorted(sorted));
//	}
//}