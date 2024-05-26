#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

EXE = gui
IMGUI_DIR = ./imgui
UTILS_DIR = ./utils
DS_DIR = ./structures/lists
DA_DIR = $(DS_DIR)/dynamic_array
SLL_DIR = $(DS_DIR)/singly_linked_list
DSL_DIR = $(DS_DIR)/doubly_linked_list
QUEUE_DIR = ./structures/queue
DICT_DIR = ./structures/dictionary
HASH_DIR = ./structures/dictionary/hash_tables
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp $(UTILS_DIR)/browser.cpp $(UTILS_DIR)/generators.cpp ./structures/validate.cpp $(DA_DIR)/dynamic_array.cpp $(DS_DIR)/singly_linked_list.cpp $(DS_DIR)/singly_linked_list_tail.cpp $(DSL_DIR)/doubly_linked_list.cpp $(QUEUE_DIR)/heap_queue/heap_queue.cpp $(QUEUE_DIR)/bst_queue/bst_queue.cpp $(DICT_DIR)/adt.cpp $(DICT_DIR)/red_black_tree/red_black_tree.cpp $(HASH_DIR)/closed_addressing.cpp $(HASH_DIR)/coalesced.cpp $(HASH_DIR)/cuckoo.cpp $(HASH_DIR)/open_addressing.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++17 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(UTILS_DIR) -I./structures  -I$(DS_DIR) -I$(DA_DIR) -I$(SLL_DIR) -I$(DSL_DIR) -I$(QUEUE_DIR) -I$(QUEUE_DIR)/heap_queue -I$(QUEUE_DIR)/bst_queue -I$(DICT_DIR) -I$(DICT_DIR)/red_black_tree -I$(HASH_DIR)
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(UTILS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/lists/dynamic_array/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/lists/singly_linked_list/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/lists/doubly_linked_list/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/queue/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/queue/heap_queue/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/queue/bst_queue/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/dictionary/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/dictionary/red_black_tree/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/dictionary/hash_tables/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:structures/validate.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
