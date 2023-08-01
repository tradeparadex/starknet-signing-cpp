cmake_minimum_required (VERSION 3.5)

project(tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)

add_executable(test test.cpp TestUtils.cpp)
target_link_libraries(test signer gtest gtest_main gmock pthread)
add_test(test test)

add_executable(clientTest ClientTest.cpp)
target_link_libraries(clientTest curlpp_static signer)

add_executable(algo_example algo_example.cpp)
target_link_libraries(algo_example curlpp_static signer)

add_executable(perf_test perf_test.cpp)
target_link_libraries(perf_test curlpp_static signer)
