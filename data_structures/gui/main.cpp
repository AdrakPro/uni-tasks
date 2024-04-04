// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "utils/browser.h"
#include "utils/number_generator.h"

#include <chrono>

#include "data_structures/lists/dynamic_array/dynamic_array.h"
#include "data_structures/lists/singly_linked_list/singly_linked_list.h"
#include "data_structures/lists/singly_linked_list/singly_linked_list_tail.h"
#include "data_structures/lists/doubly_linked_list/doubly_linked_list.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
	fprintf(
			stderr,
			"GLFW Error %d: %s\n",
			error,
			description
	);
}

// Measure functions
template<typename T, typename Func>
long performOperation(const T &structure, int operations_number, Func operation) {
	std::vector<T> list(operations_number, structure);
	std::chrono::high_resolution_clock::time_point start, end;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < operations_number; ++i) {
		operation(list[i]);
	}
	end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::microseconds>(
			end - start
	).count();

	return time;
}

template<typename T, typename Func>
void addButtonCallback(const T &structure, const std::string &buttonLabel,
											 const Func &func, const std::string &actionDescription,
											 std::vector<std::string> &history) {
	if (ImGui::Button(buttonLabel.c_str(), ImVec2(190.0f, 50.0f))) {
		long time = performOperation(
				structure, 1000,
				[&func](T &structure) {
					func(structure);
				}
		);
		history.push_back(
				actionDescription + " took " + std::to_string(time) + " \xC2\xB5s!"
		);
	}
}

// Main code
int main(int, char**) {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(
			1280, 720,
			"Data structures analysis",
			nullptr,
			nullptr
	);

	if (window == nullptr) {
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Init file browser
	Browser fileBrowser;
	fileBrowser.init();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.ItemSpacing = ImVec2(12.0f, 12.0f);
	style.FramePadding = ImVec2(4.0f, 6.0f);

	//	Color palette
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
	colors[ImGuiCol_Border] = ImVec4(0.32f, 0.82f, 0.45f, 0.50f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.32f, 0.82f, 0.45f, 0.86f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.84f, 0.45f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
	colors[ImGuiCol_Button] = ImVec4(0.32f, 0.82f, 0.45f, 0.86f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.78f, 0.45f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.78f, 0.45f, 1.00f);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);

#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);

	// STATE
	ImVec4 bg_color(0.20f, 0.20f, 0.20f, 1.0f);
	const ImVec2 offset(50.0f, 50.0f);
	const ImVec2 button_size(190.0f, 50.0f);
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
																 ImGuiWindowFlags_NoCollapse;

	int* data = nullptr;
	int id = -1;
	int size = 0;
	int random_index = 0;
	std::vector<std::string> history;

	// MAIN LOOP
#ifdef __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = nullptr;
	EMSCRIPTEN_MAINLOOP_BEGIN
#else
	while (!glfwWindowShouldClose(window))
#endif
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Define positions and sizes of windows (TODO: fix responsive)
		const ImVec2 viewport_size = io.DisplaySize;

		const ImVec2 data_window_size(
				viewport_size.x / 6.0f, viewport_size.y / 2.0f
		);
		const ImVec2 structure_window_size(
				viewport_size.x / 2.07f, viewport_size.y / 2.0f
		);
		const ImVec2 operations_window_size(
				viewport_size.x / 2.07f, viewport_size.y / 3.0f
		);
		const ImVec2 history_window_size(
				viewport_size.x / 5.0f, viewport_size.y - 2 * offset.y
		);

		ImVec2 left_pos(offset.x, offset.y);
		ImVec2 center_pos(data_window_size.x + 2 * offset.x, offset.y);
		ImVec2 under_center_pos(
				center_pos.x, (viewport_size.y / 2.0f) + 2 * offset.y
		);
		ImVec2 right_pos(
				(data_window_size.x + structure_window_size.x + 3 * offset.x), offset.y
		);

//		ImGui::ShowDemoWindow(nullptr);

		// DATA WINDOW
		ImGui::SetNextWindowPos(left_pos);
		ImGui::SetNextWindowSize(data_window_size, ImGuiCond_Once);
		ImGui::Begin("Data", nullptr, flags);

		if (ImGui::Button("Load from file (txt)", button_size)) {
			fileBrowser.open();
		}

		if (ImGui::Button("Generate random data", button_size)) {
			ImGui::OpenPopup("Generate random data");
		}
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal(
				"Generate random data", nullptr, ImGuiWindowFlags_AlwaysAutoResize
		)) {
			ImGui::Text("How much data to generate?");
			ImGui::InputInt("##", &size, 1, 100);
			ImGui::Separator();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PopStyleVar();

			if (ImGui::Button("Ok", ImVec2(120, 0)) && size > 0) {
				delete[] data;
				data = generateNumbers(size, 50);
				Browser::save(data, size);
				random_index = generateNumber(0, size, 50);
				std::cout << "Generated index: " << random_index << std::endl;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End(); // END DATA WINDOW

		fileBrowser.selectPathAndLoad(data, size);

		// DATA STRUCTURE WINDOW
		ImGui::SetNextWindowPos(center_pos);
		ImGui::SetNextWindowSize(structure_window_size, ImGuiCond_Once);
		ImGui::Begin(
				"Data structures", nullptr, flags
		);

		if (ImGui::Button("Dynamic array", button_size)) {
			history.clear();
			id = 0;
		}

		ImGui::SameLine();

		if (ImGui::Button("Linked list (head)", button_size)) {
			history.clear();
			id = 1;
		}

		ImGui::SameLine();

		if (ImGui::Button("Linked list (head, tail)", button_size)) {
			history.clear();
			id = 2;
		}

		if (ImGui::Button("Double linked list", button_size)) {
			history.clear();
			id = 3;
		}

		ImGui::End();

		ImGui::SetNextWindowPos(under_center_pos);
		ImGui::SetNextWindowSize(operations_window_size, ImGuiCond_Once);
		ImGui::Begin(
				"Operations", nullptr, flags
		);

		if (data != nullptr) {
			int number_to_add = 100;
			int random_element;

			switch (id) {
				case 0: {
					auto* dynamic_array = new DynamicArray(data, size);
					random_element = dynamic_array->getElement(random_index);

					addButtonCallback(
							*dynamic_array, "Add front", [number_to_add](DynamicArray &arr) {
								arr.addFront(number_to_add);
							}, "Add front", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Add back", [number_to_add](DynamicArray &arr) {
								arr.addBack(number_to_add);
							}, "Add back", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Add", [number_to_add, random_index](
									DynamicArray &arr) {
								arr.add(
										number_to_add, random_index
								);
							}, "Add", history
					);
					addButtonCallback(
							*dynamic_array, "Remove front",
							[](DynamicArray &arr) { arr.removeFront(); }, "Remove front",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Remove back",
							[](DynamicArray &arr) { arr.removeBack(); }, "Remove back",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Remove", [random_index](DynamicArray &arr) {
								arr.remove(random_index);
							}, "Remove", history
					);
					addButtonCallback(
							*dynamic_array, "Find", [random_element](DynamicArray &arr) {
								arr.find(random_element);
							}, "Find", history
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						dynamic_array->display();
					}
					break;
				}
				case 1: {
					auto* linked_list = new SLinkedList(data, size);
					random_element = linked_list->getNodeValue(random_index);

					addButtonCallback(
							*linked_list, "Add front", [number_to_add](SLinkedList &list) {
								list.addFront(number_to_add);
							}, "Add front", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Add back", [number_to_add](SLinkedList &list) {
								list.addBack(number_to_add);
							}, "Add back", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Add", [number_to_add, random_index](
									SLinkedList &list) { list.add(number_to_add, random_index); },
							"Add", history
					);
					addButtonCallback(
							*linked_list, "Remove front",
							[](SLinkedList &list) { list.removeFront(); }, "Remove front",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Remove back",
							[](SLinkedList &list) { list.removeBack(); }, "Remove back",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Remove", [random_index](SLinkedList &list) {
								list.remove(random_index);
							}, "Remove", history
					);
					addButtonCallback(
							*linked_list, "Find", [random_element](SLinkedList &list) {
								list.find(random_element);
							}, "Find", history
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						linked_list->display();
					}
					break;
				}
				case 2: {
					auto* linked_list_tail = new SLinkedListWithTail(data, size);
					random_element = linked_list_tail->getNodeValue(random_index);

					addButtonCallback(
							*linked_list_tail, "Add front", [number_to_add](
									SLinkedListWithTail &list) { list.addFront(number_to_add); },
							"Add front", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Add back", [number_to_add](
									SLinkedListWithTail &list) { list.addBack(number_to_add); },
							"Add back", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Add", [number_to_add, random_index](
									SLinkedListWithTail &list) {
								list.add(
										number_to_add, random_index
								);
							}, "Add", history
					);
					addButtonCallback(
							*linked_list_tail, "Remove front",
							[](SLinkedListWithTail &list) { list.removeFront(); },
							"Remove front", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Remove back",
							[](SLinkedListWithTail &list) { list.removeBack(); },
							"Remove back", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Remove", [random_index](
									SLinkedListWithTail &list) { list.remove(random_index); },
							"Remove", history
					);
					addButtonCallback(
							*linked_list_tail, "Find", [random_element](
									SLinkedListWithTail &list) { list.find(random_element); },
							"Find", history
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						linked_list_tail->display();
					}
					break;
				}

				case 3: {
					auto* double_linked_list = new DLinkedList(data, size);
					random_element = double_linked_list->getNodeValue(random_index, true);

					addButtonCallback(
							*double_linked_list, "Add front",
							[number_to_add](DLinkedList &list) {
								list.addFront(number_to_add);
							}, "Add front", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Add back",
							[number_to_add](DLinkedList &list) {
								list.addBack(number_to_add);
							}, "Add back", history
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Add", [number_to_add, random_index](
									DLinkedList &list) {
								list.add(
										number_to_add, random_index
								);
							}, "Add", history
					);
					addButtonCallback(
							*double_linked_list, "Remove front",
							[](DLinkedList &list) { list.removeFront(); }, "Remove front",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Remove back",
							[](DLinkedList &list) { list.removeBack(); }, "Remove back",
							history
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Remove", [random_index](DLinkedList &list) {
								list.remove(random_index);
							}, "Remove", history
					);
					addButtonCallback(
							*double_linked_list, "Find", [random_element](DLinkedList &list) {
								list.find(random_element);
							}, "Find", history
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						double_linked_list->display();
					}
					break;
				}
				default:
					break;
			}
		}
		ImGui::End(); // END DATA STRUCTURE WINDOW

		// HISTORY WINDOW
		ImGui::SetNextWindowPos(right_pos);
		ImGui::SetNextWindowSize(history_window_size, ImGuiCond_Once);
		ImGui::Begin(
				"History", nullptr,
				flags
		);

		for (size_t i = 0; i < history.size(); ++i) {
			ImGui::Text("[%zu] %s", i, history[i].c_str());
		}

		ImGui::End(); // END HISTORY WINDOW

		// RENDERING
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(
				bg_color.x * bg_color.w,
				bg_color.y * bg_color.w,
				bg_color.z * bg_color.w,
				bg_color.w
		);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

// CLEANUP
	ImGui_ImplOpenGL3_Shutdown();

	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}
