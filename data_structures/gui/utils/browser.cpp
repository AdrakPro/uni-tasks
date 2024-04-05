#include "browser.h"

void Browser::init() {
	browser.SetTitle("Select data file to load");
	browser.SetTypeFilters({".txt"});
}

void Browser::open() {
	browser.Open();
}

void Browser::selectPathAndLoad(int* &array, int &size, int &random_index, const std::function<void()>& reset) {
	std::string path;

	browser.Display();

	if (browser.HasSelected()) {
		path = browser.GetSelected().string();
		std::cout << "Selected file: " << path << std::endl;
		browser.ClearSelected();
		load(path, array, size, random_index);
		reset();
	}
}

void Browser::load(const std::string &path, int* &array, int &size, int &random_index) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Error opening file!" << std::endl;
		return;
	}

	loadedData.clear();

	int lines;

	while (file >> lines) {
		loadedData.push_back(lines);
	}

	file.close();

	delete[] array;
	size = 0;

	array = new int[loadedData.size()];

	for (size_t i = 0; i < loadedData.size(); ++i) {
		array[i] = loadedData[i];
		++size;
	}

	random_index = generateNumber(0, size, 50);

	std::cout << "Generated index: " << random_index << std::endl;
}

void Browser::save(const int* data, int size) {
	const std::string fileName = "data/data.txt";
	std::ofstream file(fileName, std::ios::trunc);

	if (!file.is_open()) {
		std::cerr << "Error opening file for writing! " << std::endl;
		return;
	}

	for (int i = 0; i < size; ++i) {
		file << data[i] << "\n";
	}

	file.close();
}