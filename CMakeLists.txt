cmake_minimum_required(VERSION 3.12)

project(EyeCAN VERSION 0.1)

# CONFIGURATION
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
# Search for dbcppp library and include directories
find_library(DBCPPP_LIBRARY NAMES dbcppp
        PATHS "$ENV{HOME}/.local/lib" "/usr/local/lib" "/usr/lib"
        PATH_SUFFIXES ""
)

find_path(DBCPPP_INCLUDE_DIR dbcppp/Attribute.h
        PATHS "$ENV{HOME}/.local/include" "/usr/local/include" "/usr/include"
        PATH_SUFFIXES ""
)
# Option to enable or disable tests
option(ENABLE_TESTS "Enable building tests" ON)

# Handle dbcppp discovery
if (DBCPPP_LIBRARY AND DBCPPP_INCLUDE_DIR)
    message(STATUS "Found dbcppp: ${DBCPPP_LIBRARY}")
else()
    message(WARNING "dbcppp not found. Using local dbcppp source files.")
    set(DBCPPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dbcppp/include")
endif()

# Search for mdflib library
find_library(MDFLIB_LIBRARY NAMES mdflibrary
        PATHS "$ENV{HOME}/.local/lib" "/usr/local/lib" "/usr/lib"
)

# Search for mdflib include directory
find_path(MDFLIB_INCLUDE_DIR NAMES mdf/mdfreader.h
        PATHS "$ENV{HOME}/.local/include" "/usr/local/include" "/usr/include"
)

# Handle mdflib discovery
if (MDFLIB_LIBRARY AND MDFLIB_INCLUDE_DIR)
    message(STATUS "Found mdflib library: ${MDFLIB_LIBRARY}")
    message(STATUS "Found mdflib include directory: ${MDFLIB_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "mdflib not found! Ensure it is installed correctly.")
endif()

endif ()

# Find all source files in the source directory
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/source/*.cpp)

# Find all header files in the include directory
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

# Define executable
add_executable(EyeCAN main.cpp ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(EyeCAN PRIVATE ${CMAKE_SOURCE_DIR}/libraries/json/include ${CMAKE_SOURCE_DIR}/libraries/cpp-httplib ${CMAKE_SOURCE_DIR}/include)

# Include directories
target_include_directories(EyeCAN PUBLIC "include" ${DBCPPP_INCLUDE_DIR} ${MDFLIB_INCLUDE_DIR} )

target_link_directories(EyeCAN PRIVATE
        /usr/lib/gcc/x86_64-linux-gnu/9
        /lib/x86_64-linux-gnu
)

# Link libraries if found
if (DBCPPP_LIBRARY)
    target_link_libraries(EyeCAN PRIVATE ${DBCPPP_LIBRARY})
endif()

if (MDFLIB_LIBRARY)
    target_link_libraries(EyeCAN PRIVATE ${MDFLIB_LIBRARY})
endif()

target_link_libraries(EyeCAN PRIVATE pthread)

if (ws2_32)
    target_link_libraries(EyeCAN PRIVATE ws2_32)
endif()



# Enable static linking only for dbcppp and mdflib (optional)
if (DBCPPP_LIBRARY OR MDFLIB_LIBRARY)
    set_target_properties(EyeCAN PROPERTIES LINK_SEARCH_START_STATIC OFF)
endif()
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
    add_executable(EyeCAN_tests test/test_api_endpoints.cpp)
    target_link_libraries(EyeCAN_tests PRIVATE ${GTEST_LIBRARIES} ws2_32)

    # Register the test
    add_test(NAME APITest COMMAND EyeCAN_tests)

    # Enable Google Test discovery
    include(GoogleTest)
    gtest_discover_tests(EyeCAN_tests)
endif()