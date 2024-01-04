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

if(NOT ${WIN_32})
    list(APPEND OPTIONAL_LIBS pthread)
endif()

add_executable(test test.cpp TestUtils.cpp)
target_link_libraries(test signer gtest gtest_main gmock ${OPTIONAL_LIBS})
add_test(test test)

add_executable(clientTest ClientTest.cpp)
target_link_libraries(clientTest nlohmann_json::nlohmann_json curlpp_static signer)