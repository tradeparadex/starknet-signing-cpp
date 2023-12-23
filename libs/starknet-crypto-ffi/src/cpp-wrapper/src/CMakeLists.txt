cmake_minimum_required(VERSION 3.5)
project(StarkwareCppWrapper CXX)
file(GLOB StarkwareCppWrapperSources *.cpp *.hpp)

include_directories(${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)
add_library(starkware_cpp_wrapper STATIC ${StarkwareCppWrapperSources})

set_target_properties(starkware_cpp_wrapper PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(starkware_cpp_wrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(starkware_cpp_wrapper starkware_rust_api algebra crypto starkex)