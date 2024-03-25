#ifndef GUI_BROWSER_H
#define GUI_BROWSER_H

#include "../imgui/imfilebrowser.h"
#include <iostream>

class Browser {
public:
	Browser() : browser(ImGui::FileBrowser()) {}

private:
	ImGui::FileBrowser browser;

public:
	void init();
	void open();
	std::string getSelectedItemPath();
};

#endif // GUI_BROWSER_H