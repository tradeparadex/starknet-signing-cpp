cmake_minimum_required(VERSION 3.5)
project(MessageSignature CXX)

file(GLOB MessageSignatureSources *.cpp *.hpp)

add_library(signer STATIC ${MessageSignatureSources})

target_include_directories(signer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(signer PUBLIC ${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)

# link starkware static libs
target_link_libraries(signer starkware_cpp_wrapper starkware_rust_api keccak curlpp_static algebra crypto starkex)