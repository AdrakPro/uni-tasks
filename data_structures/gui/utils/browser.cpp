#include "browser.h"

void Browser::init() {
	this->browser.SetTitle("Select data file to load");
	this->browser.SetTypeFilters({ ".txt", ".json" });
}

void Browser::open() {
	this->browser.Open();
}


std::string Browser::getSelectedItemPath() {
	std::string path;

	this->browser.Display();

	if (this->browser.HasSelected()) {
		path = this->browser.GetSelected().string();
		std::cout << "Selected path: " << path << std::endl;
		browser.ClearSelected();
	}

	return path;
}
