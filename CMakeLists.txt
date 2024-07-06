cmake_minimum_required(VERSION 3.20)

project(unhandled_dump)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_COMMONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" CACHE STRING "Path to common cmake scripts")
include(${CMAKE_COMMONS_DIR}/paths_utils.cmake)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Add subdirectories for the projects
add_subdirectory(unhandled_dump)
add_subdirectory(cli_test)
add_subdirectory(test)