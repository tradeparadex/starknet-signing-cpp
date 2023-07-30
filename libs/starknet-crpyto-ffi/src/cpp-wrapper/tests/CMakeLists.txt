cmake_minimum_required (VERSION 3.5)

project(CppWrapperTest LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)

add_executable(cppWrapperTest test.cpp)
target_link_libraries(cppWrapperTest starkware_cpp_wrapper starkware_rust_api)