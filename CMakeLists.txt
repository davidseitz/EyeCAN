cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-lstdc++fs)
endif()

include_directories(${CMAKE_SOURCE_DIR}/libraries/json/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/cpp-httplib)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add CPR as a submodule
set(CPR_DIR ${CMAKE_SOURCE_DIR}/libraries/cpr)
add_subdirectory(${CPR_DIR})

# Option to enable or disable tests
#option(ENABLE_TESTS "Enable building tests" ON)

add_executable(EyeCAN main.cpp
        include/eyecanserver.h
        source/eyecanserver.cpp)

# Use the keyword signature for target_link_libraries
target_link_libraries(EyeCAN PRIVATE ws2_32 cpr::cpr)


# Enable testing
enable_testing()

# Add Google Test
set(GTEST_DIR ${CMAKE_SOURCE_DIR}/libraries/gtest)
add_subdirectory(${GTEST_DIR})

# Add test executable
add_executable(EyeCAN_tests test/test_api_endpoints.cpp)
target_link_libraries(EyeCAN_tests PRIVATE GTest::gtest GTest::gtest_main cpr::cpr)

# Register the test
add_test(NAME APITest COMMAND EyeCAN_tests)

# Enable Google Test discovery
include(GoogleTest)
gtest_discover_tests(EyeCAN_tests)
