cmake_minimum_required(VERSION 3.12)
project(graduation_project)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(graduation_project main.cpp colors.cpp colors.h)
target_link_libraries(graduation_project ${OpenCV_LIBS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/utkubelemir/Desktop")