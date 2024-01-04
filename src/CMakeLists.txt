cmake_minimum_required(VERSION 3.5)
project(MessageSignature CXX)

file(GLOB MessageSignatureSources *.cpp *.hpp)

add_library(signer STATIC ${MessageSignatureSources})

target_include_directories(signer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(signer
    PUBLIC
        algebra crypto keccak
    PRIVATE
        starkware_cpp_wrapper
        starkware_rust_api
        curlpp_static
        nlohmann_json::nlohmann_json
)