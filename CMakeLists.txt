cmake_minimum_required(VERSION 3.12)

project(EyeCAN VERSION 0.1)

# CONFIGURATION
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific configurations
if (WIN32)
    set(CMAKE_C_COMPILER "C:/Program Files/JetBrains/CLion 2024.3.1.1/bin/mingw/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "C:/Program Files/JetBrains/CLion 2024.3.1.1/bin/mingw/bin/g++.exe")
    set(INSTALL_PREFIX "C:/Users/$ENV{USERNAME}/local")  # Adjust this for your Windows path
elseif (UNIX)
    target_link_directories(EyeCAN PRIVATE
            /usr/lib/gcc/x86_64-linux-gnu/9
            /lib/x86_64-linux-gnu
    )
    set(INSTALL_PREFIX "$ENV{HOME}/.local")
endif()

# Replace with the actual path where Boost is installed
set(BOOST_ROOT "C:/Program Files/boost/boost_1_87_0")
find_package(Boost REQUIRED)


message(STATUS "Using INSTALL_PREFIX: ${INSTALL_PREFIX}")

# Search for dbcppp library and include directories
find_library(DBCPPP_LIBRARY NAMES dbcppp
        PATHS "${INSTALL_PREFIX}/lib"
        "${CMAKE_SOURCE_DIR}/dbcppp/lib"  # Fallback to source directory
)

find_path(DBCPPP_INCLUDE_DIR dbcppp/Attribute.h
        PATHS "${INSTALL_PREFIX}/include"
        "${CMAKE_SOURCE_DIR}/dbcppp/include"  # Fallback to source directory
)

# Handle dbcppp discovery
if (DBCPPP_LIBRARY AND DBCPPP_INCLUDE_DIR)
    message(STATUS "Found dbcppp: ${DBCPPP_LIBRARY}")
else()
    message(WARNING "dbcppp not found. Using local dbcppp source files.")
    set(DBCPPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dbcppp/include")
endif()

# Search for mdflib library and include directories
find_library(MDFLIB_LIBRARY NAMES mdflibrary
        PATHS "${INSTALL_PREFIX}/lib"
        "${CMAKE_SOURCE_DIR}/mdflib/lib"  # Fallback to source directory
)

find_path(MDFLIB_INCLUDE_DIR NAMES mdf/mdfreader.h
        PATHS "${INSTALL_PREFIX}/include"
        "${CMAKE_SOURCE_DIR}/mdflib/include"  # Fallback to source directory
)

# Handle mdflib discovery
if (MDFLIB_LIBRARY AND MDFLIB_INCLUDE_DIR)
    message(STATUS "Found mdflib library: ${MDFLIB_LIBRARY}")
    message(STATUS "Found mdflib include directory: ${MDFLIB_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "mdflib not found! Ensure it is installed correctly.")
endif()

# Collect all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/source/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

# Define executable
add_executable(EyeCAN main.cpp ${SOURCE_FILES} ${HEADER_FILES})

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/libraries/json/include
        ${CMAKE_SOURCE_DIR}/libraries/cpp-httplib
        ${CMAKE_SOURCE_DIR}/include
        ${DBCPPP_INCLUDE_DIR}
        ${MDFLIB_INCLUDE_DIR}
)

target_link_libraries(EyeCAN PRIVATE ${DBCPPP_LIBRARY} ${MDFLIB_LIBRARY})

if (WIN32)
    target_link_libraries(EyeCAN PRIVATE ws2_32)
endif()

# Enable testing if requested
option(ENABLE_TESTS "Enable building tests" ON)
if (ENABLE_TESTS)
    enable_testing()
    set(GTEST_ROOT "C:/Program Files (x86)/googletest-distribution")
    set(GTEST_INCLUDE_DIRS "${GTEST_ROOT}/include")
    set(GTEST_LIBRARIES "${GTEST_ROOT}/lib/libgtest.a" "${GTEST_ROOT}/lib/libgtest_main.a")

    include_directories(${GTEST_INCLUDE_DIRS})
    file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)
    add_executable(EyeCAN_tests ${TEST_FILES} ${SOURCE_FILES} ${HEADER_FILES})
    target_link_libraries(EyeCAN_tests PRIVATE ${GTEST_LIBRARIES} ws2_32)
    add_test(NAME APITest COMMAND EyeCAN_tests)
    include(GoogleTest)
    gtest_discover_tests(EyeCAN_tests)
endif()
