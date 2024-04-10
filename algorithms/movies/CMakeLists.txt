cmake_minimum_required(VERSION 3.26.3)
project(movies)

set(CMAKE_CXX_STANDARD 17)

include_directories(tests)

add_executable(movies main.cpp
        model/movie.h
        model/movie.cpp
        utils/browser.h
        utils/browser.cpp
        tests/catch.hpp
        algorithms/sorting.h
        algorithms/sorting.cpp
)
