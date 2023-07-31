cmake_minimum_required(VERSION 3.5)
project(StarkwareCppWrapper CXX)
file(GLOB StarkwareCppWrapperSources *.cpp *.hpp)

include_directories(${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)
add_library(starkware_cpp_wrapper STATIC ${StarkwareCppWrapperSources})

# link starkware static libs
set (STARKWARE_ALGEBRA ${CMAKE_SOURCE_DIR}/libs/crypto-cpp/build/src/starkware/algebra/libalgebra.a)
set (STARKWARE_CRYPTO ${CMAKE_SOURCE_DIR}/libs/crypto-cpp/build/src/starkware/crypto/libcrypto.a)
set (STARKWARE_STARKEX ${CMAKE_SOURCE_DIR}/libs/crypto-cpp/build/src/starkware/starkex/libstarkex.a)
set (STATIC_LIBS ${STARKWARE_STARKEX} ${STARKWARE_CRYPTO} ${STARKWARE_ALGEBRA})

set_target_properties(starkware_cpp_wrapper PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(starkware_cpp_wrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(starkware_cpp_wrapper starkware_rust_api ${STATIC_LIBS})