cmake_minimum_required(VERSION 3.12)
project(opencv_color_1)
set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/Users/utkubelemir/Desktop")

add_executable(opencv_color_1 main.cpp colors.cpp colors.h)
target_link_libraries(opencv_color_1 ${OpenCV_LIBS})