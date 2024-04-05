#ifndef GUI_BROWSER_H
#define GUI_BROWSER_H

#include "../imgui/imfilebrowser.h"
#include <iostream>
#include <fstream>
#include "number_generator.h"

class Browser {
private:
	ImGui::FileBrowser browser;
	std::vector<int> loadedData;

public:
	Browser() : browser(ImGui::FileBrowser()) {}

	void init();

	void open();

	void selectPathAndLoad(int* &array, int &size, int &random_index, const std::function<void()>& reset);

	void load(const std::string &path, int* &array, int &size, int &random_index);

	static void save(const int* data, int size);
};

#endif // GUI_BROWSER_H