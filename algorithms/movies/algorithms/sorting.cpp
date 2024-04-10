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

std::vector<Movie> Sorting::mergeSort(std::vector<Movie> &data) {
	int size = static_cast<int>(data.size());

	if (size <= 1) {
		return data;
	}

	int mid = size / 2;

	std::vector<Movie> left_half(data.begin(), data.begin() + mid);
	std::vector<Movie> right_half(data.begin() + mid, data.end());

	std::vector<Movie> left = mergeSort(left_half);
	std::vector<Movie> right = mergeSort(right_half);

	return merge(left, right);
}

void Sorting::swap(std::vector<Movie> &data, int i1, int i2) {
	if (i1 != i2) {
		Movie aux = data[i1];
		data[i1] = data[i2];
		data[i2] = aux;
	}
}

int Sorting::medianOfThree(std::vector<Movie> &data, int left, int right) {
	int mid = left + (right - left) / 2;

	if (data[right].rating < data[left].rating) {
		swap(data, left, right);
	}

	if (data[mid].rating < data[left].rating) {
		swap(data, left, mid);
	}

	if (data[right].rating < data[mid].rating) {
		swap(data, mid, right);
	}

	return mid;
}

std::vector<Movie> Sorting::quickSort(std::vector<Movie> &data, int low, int high) {
	if (high <= low + 16 - 1) {
		insertionSort(data, low, high);
		return data;
	}

	int median = medianOfThree(data, low, high);
	swap(data, low, median);
	int partition_index = partition(data, low, high);
	quickSort(data, low, partition_index - 1);
	quickSort(data, partition_index + 1, high);

	return data;
}

int Sorting::partition(std::vector<Movie> &data, int low, int high) {
	int i = low, j = high + 1;
	int v = data[low].rating;

	while (true) {
		while (data[++i].rating < v) {
			if (i == high) break;
		}
		while (v < data[--j].rating) {
			if (j == low) break;
		}
		if (i >= j) break;
		swap(data, i, j);
	}
	swap(data, low, j);
	return j;
}

// Choose mid index
int Sorting::choosePivot(int left, int right) {
	return left + (right - left) / 2;
}

// std::list.sort is O(n log n) https://cplusplus.com/reference/list/list/sort/
void Sorting::nextSort(std::list<Movie> &bucket) {
	bucket.sort(
			[](const Movie &a, const Movie &b) {
				return a.rating <= b.rating;
			}
	);
}

//16004799
std::vector<Movie> Sorting::bucketSort(std::vector<Movie> &data, int buckets_num) {
	std::vector<std::list<Movie>> buckets(buckets_num);
	int max_rating = 0;

	for (const Movie &movie: data) {
		if (movie.rating > max_rating) {
			max_rating = movie.rating;
		}
	}

	for (const Movie &movie: data) {
		int index = static_cast<int>(buckets_num * movie.rating / (max_rating + 1));
		buckets[index].push_back(movie);
	}

	for (std::list<Movie> &bucket: buckets) {
		nextSort(bucket);
	}

	std::vector<Movie> sorted;
	for (const std::list<Movie> &bucket: buckets) {
		sorted.insert(sorted.end(), bucket.begin(), bucket.end());
	}

	return sorted;
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

	return data;
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

	return data;
}

std::vector<Movie> Sorting::introSort(std::vector<Movie> &data, int max_depth) {
	int size = static_cast<int>(data.size());
	int low = 0;
	int high = size - 1;

	if (size <= 16) {
		insertionSort(data, low, high);
	} else if (max_depth == 0) {
		heapSort(data);
	} else {
		int partition_index = partition(data, low, high);

		std::vector<Movie> left_partition(data.begin(), data.begin() + partition_index);
		std::vector<Movie> right_partition(data.begin() + partition_index + 1, data.end());

		introSort(left_partition, max_depth - 1);
		introSort(right_partition, max_depth - 1);
	}

	return data;
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