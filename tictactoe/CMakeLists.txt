cmake_minimum_required(VERSION 3.27)
project(tictactoe)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(tictactoe main.cpp
        game.cpp
        game.h
)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
)