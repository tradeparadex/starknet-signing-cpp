cmake_minimum_required (VERSION 3.5)

project(tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/libs/crypto-cpp/src)

add_executable(test test.cpp)
target_link_libraries(test signer gtest gtest_main gmock pthread)
add_test(test test)

add_executable(clientTest ClientTest.cpp)
target_link_libraries(clientTest curlpp_static signer)