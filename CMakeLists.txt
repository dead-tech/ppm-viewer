cmake_minimum_required(VERSION 3.23)
project(ppm_viewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wconversion -Wpedantic -fsanitize=undefined")

include_directories(ppm_viewer "${CMAKE_SOURCE_DIR}/include/")

add_executable(ppm_viewer ${CMAKE_SOURCE_DIR}/src/main.cpp ${CMAKE_SOURCE_DIR}/src/PPMParser.cpp)
