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

#include "structures/lists/dynamic_array/dynamic_array.h"
#include "structures/lists/singly_linked_list/singly_linked_list.h"
#include "structures/lists/singly_linked_list/singly_linked_list_tail.h"
#include "structures/lists/doubly_linked_list/doubly_linked_list.h"

#include "structures/queue/heap_queue/heap_queue.h"

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

// GLOBALS
const int NUMBER_OF_SAMPLES = 12;
const int OPERATIONS_PER_SAMPLE = 50;

int* data = nullptr;
int size = 0;
int random_index = 0;
int id = -1;
std::vector<std::string> history;
std::vector<int*> cached_data;

DynamicArray* dynamic_array = nullptr;
SLinkedList* linked_list = nullptr;
SLinkedListWithTail* linked_list_tail = nullptr;
DLinkedList* double_linked_list = nullptr;
MaxHeapPriorityQueue* max_heap_queue = nullptr;

void reset() {
	cached_data.clear();
	history.clear();
	id = -1;

	dynamic_array = nullptr;
	linked_list = nullptr;
	linked_list_tail = nullptr;
	double_linked_list = nullptr;
	max_heap_queue = nullptr;
}

// Measure functions
template<typename T, typename Func>
long performOperation(T &structure, Func operation) {
	std::vector<std::unique_ptr<T>> list;

	list.reserve(OPERATIONS_PER_SAMPLE);

	for (int i = 0; i < OPERATIONS_PER_SAMPLE; ++i) {
		list.push_back(std::make_unique<T>(structure));
	}

	std::chrono::high_resolution_clock::time_point start, end;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < OPERATIONS_PER_SAMPLE; ++i) {
		operation(*list[i]);
	}
	end = std::chrono::high_resolution_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
			end - start
	).count();

	list.clear();

	return time;
}

template<typename T, typename Func>
void addButtonCallback(T &structure, const std::string &buttonLabel, const Func &func) {
	if (ImGui::Button(buttonLabel.c_str(), ImVec2(190.0f, 50.0f))) {
		long result = 0;

		for (int i = 0; i < NUMBER_OF_SAMPLES; ++i) {
			long time = performOperation(
					structure,
					[&func](T &structure) {
						func(structure);
					}
			);

			result += time;

			structure.setData(cached_data[i], size);
		}

		result /= NUMBER_OF_SAMPLES;

		history.push_back(
				buttonLabel + " took avg " + std::to_string(result) + " ns!"
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

	// Color palette
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
	colors[ImGuiCol_Border] = ImVec4(0.32f, 0.82f, 0.45f, 0.50f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.32f, 0.82f, 0.45f, 0.86f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.84f, 0.45f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
	colors[ImGuiCol_Button] = ImVec4(0.32f, 0.82f, 0.45f, 0.86f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.32f, 0.78f, 0.45f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.78f, 0.45f, 1.00f);

	ImVec4 bg_color(0.20f, 0.20f, 0.20f, 1.0f);
	const ImVec2 offset(50.0f, 50.0f);
	const ImVec2 button_size(190.0f, 50.0f);
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
																 ImGuiWindowFlags_NoCollapse;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);

#ifdef __EMSCRIPTEN__
	ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
	ImGui_ImplOpenGL3_Init(glsl_version);

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
				viewport_size.x / 4.8f, viewport_size.y - 1.45f * offset.y
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
				reset();

				// Generate data, cache it and save to file
				random_index = generateNumber(0, size, 50);
				std::cout << "Generated index " << random_index << " for dataset " << size
									<< std::endl;

				cached_data.push_back(data);
				for (int i = 0; i < NUMBER_OF_SAMPLES - 1; ++i) {
					int* dataset = generateNumbers(size, 50 + 2 * i);
					cached_data.push_back(dataset);

					std::string file_name = std::to_string(size) + "_" + std::to_string(i + 1);

					if (!std::filesystem::exists("data/" + file_name + ".txt")) {
						Browser::save(dataset, size, file_name);
					}
				}

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

		fileBrowser.selectPathAndLoad(
				data, size, random_index, reset
		);

		// DATA STRUCTURE WINDOW
		ImGui::SetNextWindowPos(center_pos);
		ImGui::SetNextWindowSize(structure_window_size, ImGuiCond_Once);
		ImGui::Begin(
				"Data structures", nullptr, flags
		);

		if (ImGui::Button("Dynamic Array", button_size)) {
			history.clear();
			id = 0;
		}

		ImGui::SameLine();

		if (ImGui::Button("Linked List (head)", button_size)) {
			history.clear();
			id = 1;
		}

		ImGui::SameLine();

		if (ImGui::Button("Linked List (head, tail)", button_size)) {
			history.clear();
			id = 2;
		}


		if (ImGui::Button("Double Linked List", button_size)) {
			history.clear();
			id = 3;
		}

		ImGui::SameLine();

		if (ImGui::Button("Max Heap Priority Queue", button_size)) {
			history.clear();
			id = 4;
		}

		ImGui::End();

		ImGui::SetNextWindowPos(under_center_pos);
		ImGui::SetNextWindowSize(operations_window_size, ImGuiCond_Once);
		ImGui::Begin(
				"Operations", nullptr, flags
		);

		if (data != nullptr) {
			const int NUMBER_TO_ADD = 100;
			const QNode Q_NODE(5, 2137, 50);

			switch (id) {
				// 0-3 Lists structures
				case 0: {
					if (dynamic_array == nullptr) {
						dynamic_array = new DynamicArray();
						dynamic_array->setData(data, size);
					}

					addButtonCallback(
							*dynamic_array, "Add front", [NUMBER_TO_ADD](DynamicArray &arr) {
								arr.addFront(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Add back", [NUMBER_TO_ADD](DynamicArray &arr) {
								arr.addBack(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Add", [NUMBER_TO_ADD](
									DynamicArray &arr) {
								arr.add(
										NUMBER_TO_ADD, random_index
								);
							}
					);
					addButtonCallback(
							*dynamic_array, "Remove front",
							[](DynamicArray &arr) { arr.removeFront(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Remove back",
							[](DynamicArray &arr) { arr.removeBack(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*dynamic_array, "Remove", [](DynamicArray &arr) {
								arr.remove(random_index);
							}
					);
					addButtonCallback(
							*dynamic_array, "Find", [](DynamicArray &arr) {
								arr.find(arr.getElement(random_index));
							}
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						dynamic_array->display();
					}
					break;
				}
				case 1: {
					if (linked_list == nullptr) {
						linked_list = new SLinkedList();
						linked_list->setData(data, size);
					}

					addButtonCallback(
							*linked_list, "Add front", [NUMBER_TO_ADD](SLinkedList &list) {
								list.addFront(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Add back", [NUMBER_TO_ADD](SLinkedList &list) {
								list.addBack(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Add", [NUMBER_TO_ADD](
									SLinkedList &list) { list.add(NUMBER_TO_ADD, random_index); }
					);
					addButtonCallback(
							*linked_list, "Remove front",
							[](SLinkedList &list) { list.removeFront(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Remove back",
							[](SLinkedList &list) { list.removeBack(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list, "Remove", [](SLinkedList &list) {
								list.remove(random_index);
							}
					);
					addButtonCallback(
							*linked_list, "Find", [](SLinkedList &list) {
								list.find(list.getNodeValue(random_index));
							}
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						linked_list->display();
					}
					break;
				}
				case 2: {
					if (linked_list_tail == nullptr) {
						linked_list_tail = new SLinkedListWithTail();
						linked_list_tail->setData(data, size);
					}

					addButtonCallback(
							*linked_list_tail, "Add front", [NUMBER_TO_ADD](
									SLinkedListWithTail &list) { list.addFront(NUMBER_TO_ADD); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Add back", [NUMBER_TO_ADD](
									SLinkedListWithTail &list) { list.addBack(NUMBER_TO_ADD); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Add", [NUMBER_TO_ADD](
									SLinkedListWithTail &list) {
								list.add(
										NUMBER_TO_ADD, random_index
								);
							}
					);
					addButtonCallback(
							*linked_list_tail, "Remove front",
							[](SLinkedListWithTail &list) { list.removeFront(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Remove back",
							[](SLinkedListWithTail &list) { list.removeBack(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*linked_list_tail, "Remove", [](
									SLinkedListWithTail &list) { list.remove(random_index); }
					);
					addButtonCallback(
							*linked_list_tail, "Find", [](
									SLinkedListWithTail &list) {
								list.find(
										list.getNodeValue(random_index));
							}
					);
					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						linked_list_tail->display();
					}
					break;
				}

				case 3: {
					if (double_linked_list == nullptr) {
						double_linked_list = new DLinkedList();
						double_linked_list->setData(data, size);
					}

					addButtonCallback(
							*double_linked_list, "Add front",
							[NUMBER_TO_ADD](DLinkedList &list) {
								list.addFront(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Add back",
							[NUMBER_TO_ADD](DLinkedList &list) {
								list.addBack(NUMBER_TO_ADD);
							}
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Add", [NUMBER_TO_ADD](
									DLinkedList &list) {
								list.add(
										NUMBER_TO_ADD, random_index
								);
							}
					);
					addButtonCallback(
							*double_linked_list, "Remove front",
							[](DLinkedList &list) { list.removeFront(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Remove back",
							[](DLinkedList &list) { list.removeBack(); }
					);
					ImGui::SameLine();
					addButtonCallback(
							*double_linked_list, "Remove", [](DLinkedList &list) {
								list.remove(random_index);
							}
					);
					addButtonCallback(
							*double_linked_list, "Find", [](DLinkedList &list) {
								list.find(list.getNodeValue(random_index, true));
							}
					);

					ImGui::SameLine();

					if (ImGui::Button("Display", button_size)) {
						double_linked_list->display();
					}
					break;
				}
					// 4-5 Queue
				case 4: {
					if (max_heap_queue == nullptr) {
						max_heap_queue = new MaxHeapPriorityQueue();
						max_heap_queue->setData(data, size);
					}

					addButtonCallback(
							*max_heap_queue, "Insert",
							[Q_NODE](MaxHeapPriorityQueue &queue) {
								queue.insert(Q_NODE);
							}
					);

					ImGui::SameLine();

					addButtonCallback(
							*max_heap_queue, "Extract Max",
							[](MaxHeapPriorityQueue &queue) {
								queue.extractMax();
							}
					);

					ImGui::SameLine();

					addButtonCallback(
							*max_heap_queue, "Peek",
							[](MaxHeapPriorityQueue &queue) {
								queue.peek();
							}
					);

					addButtonCallback(
							*max_heap_queue, "Modify Key",
							[Q_NODE](MaxHeapPriorityQueue &queue) {
								queue.modifyKey(random_index, Q_NODE.priority);
							}
					);

					ImGui::SameLine();

					if (ImGui::Button("Get size", button_size)) {
						std::cout << "Size of heap: " << max_heap_queue->getSize() << std::endl;
					}

					ImGui::SameLine();
					if (ImGui::Button("Display", button_size)) {
						max_heap_queue->display();
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
			ImGui::Text("[%zu] %s", i + 1, history[i].c_str());
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
