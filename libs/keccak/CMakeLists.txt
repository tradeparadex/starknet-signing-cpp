cmake_minimum_required (VERSION 3.5)

project(keccak LANGUAGES CXX)

file(GLOB KeccakSources *.cpp *.hpp)

add_library(keccak STATIC ${KeccakSources})
target_include_directories(keccak PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})