cmake_minimum_required (VERSION 3.5)

project(ParadexLib VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(tests)
