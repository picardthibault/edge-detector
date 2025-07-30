cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(EDGE_DETECTOR VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

add_executable(Main main.cpp Image.cpp)

target_link_libraries(Main PNG::PNG ZLIB::ZLIB)

target_include_directories(Main PUBLIC "${PROJECT_BINARY_DIR}")
