cmake_minimum_required(VERSION 3.14)

project(antichain)

option(ANTICHAIN_BUILD_TEST "Build tests for Antichain" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANTICHAIN_BUILD_TEST)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0  # Adjust to version you'd like
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(RunTests_Antichain tests/Antichain_test.cpp)
target_link_libraries(RunTests_Antichain GTest::gtest_main)
target_compile_features(RunTests_Antichain PUBLIC cxx_std_20)

include(GoogleTest)
enable_testing()

gtest_discover_tests(RunTests_Antichain)
endif()