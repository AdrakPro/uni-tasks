cmake_minimum_required(VERSION 3.27)
project(dining_philosophers C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(dining_philosophers
        main.c
        utils.h
        utils.c
        text_interface.h
        text_interface.c
        philosopher.h
        philosopher.c
)

target_link_libraries(dining_philosophers ${CURSES_LIBRARIES})
