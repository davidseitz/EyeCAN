cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER "C:/Program Files/JetBrains/CLion 2024.3.1.1/bin/mingw/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/JetBrains/CLion 2024.3.1.1/bin/mingw/bin/g++.exe")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-lstdc++fs)
endif()

include_directories(${CMAKE_SOURCE_DIR}/libraries/json/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/cpp-httplib)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Option to enable or disable tests
option(ENABLE_TESTS "Enable building tests" ON)

add_executable(EyeCAN main.cpp
        include/eyeCanServer.h
        source/eyeCanServer.cpp
        include/informationHandler.h
        source/informationHandler.cpp
        include/ruleHandler.h
        source/ruleHandler.cpp
)

# Use the keyword signature for target_link_libraries
target_link_libraries(EyeCAN PRIVATE ws2_32)

# Enable testing
if (ENABLE_TESTS)
    enable_testing()

    # Manually specify the GTest paths
    set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")
    set(GTEST_INCLUDE_DIRS "${GTEST_ROOT}/include")
    set(GTEST_LIBRARIES "${GTEST_ROOT}/lib/libgtest.a" "${GTEST_ROOT}/lib/libgtest_main.a")

    include_directories(${GTEST_INCLUDE_DIRS})

    # Add test executable
    add_executable(EyeCAN_tests test/testApiEndpoints.cpp
            include/informationHandler.h
            source/informationHandler.cpp
            include/ruleHandler.h
            source/ruleHandler.cpp
            test/ruleHandlerTest.cpp
    )
    target_link_libraries(EyeCAN_tests PRIVATE ${GTEST_LIBRARIES} ws2_32)

    # Register the test
    add_test(NAME APITest COMMAND EyeCAN_tests)

    # Enable Google Test discovery
    include(GoogleTest)
    gtest_discover_tests(EyeCAN_tests)
endif()