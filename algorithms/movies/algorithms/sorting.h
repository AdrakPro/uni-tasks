#ifndef MOVIES_SORTING_H
#define MOVIES_SORTING_H

#include <list>
#include <iostream>

#include "../model/movie.h"

class Sorting {
private:
	std::vector<Movie> merge(std::vector<Movie> left, std::vector<Movie> right);

	void swap(std::vector<Movie> &data, int i1, int i2);

	int choosePivot(int left, int right);

	int partition(std::vector<Movie> &data, int low, int high);

	void nextSort(std::list<Movie> &bucket);

	int medianOfThree(std::vector<Movie>&data, int left, int right);


public:
	Sorting() {}

	bool isSorted(const std::vector<Movie> &data);

	std::vector<Movie> mergeSort(std::vector<Movie> &data);

	std::vector<Movie> quickSort(std::vector<Movie> &data, int low, int high);

	std::vector<Movie> bucketSort(std::vector<Movie> &data, int buckets_num);

	std::vector<Movie> introSort(std::vector<Movie> &data, int max_depth);

	std::vector<Movie> insertionSort(std::vector<Movie> &data, int low, int high);

	std::vector<Movie> heapSort(std::vector<Movie> &data);

	void heapify(std::vector<Movie> &data, int size, int i);
};


#endif //MOVIES_SORTING_H
