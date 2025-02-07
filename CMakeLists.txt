cmake_minimum_required(VERSION 3.12)

project(EyeCAN VERSION 0.1)

# CONFIGURATION
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Search for dbcppp library and include directories
find_library(DBCPPP_LIBRARY NAMES dbcppp
        PATHS "$ENV{HOME}/.local/lib" "/usr/local/lib" "/usr/lib"
        PATH_SUFFIXES ""
)

find_path(DBCPPP_INCLUDE_DIR dbcppp/Attribute.h
        PATHS "$ENV{HOME}/.local/include" "/usr/local/include" "/usr/include"
        PATH_SUFFIXES ""
)

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

# Define executable
add_executable(EyeCAN "src/main.cpp")

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



# Enable static linking only for dbcppp and mdflib (optional)
if (DBCPPP_LIBRARY OR MDFLIB_LIBRARY)
    set_target_properties(EyeCAN PROPERTIES LINK_SEARCH_START_STATIC OFF)
endif()
