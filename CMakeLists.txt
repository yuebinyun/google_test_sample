cmake_minimum_required(VERSION 3.20)
project(google_test_sample)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# using the GoogleTest CMake module.
include(GoogleTest)

# enables testing in CMake
enable_testing()


# --- sample1_test ------------------------------------------------------------
# declares the C++ test binary you want to build
add_executable(sample1_test sample1.cpp sample1_unittest.cpp)

# links it to GoogleTest
target_link_libraries(sample1_test gtest_main)

# enable CMake’s test runner to discover the tests included in the binary,
gtest_discover_tests(sample1_test)
# --- ------------- ------------------------------------------------------------

# --- sample2_test ------------------------------------------------------------
add_executable(sample2_test sample2.cpp sample2_unittest.cpp)
target_link_libraries(sample2_test gtest_main)
gtest_discover_tests(sample2_test)
# --- ------------- ------------------------------------------------------------



