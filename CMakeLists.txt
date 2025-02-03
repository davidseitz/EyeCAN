cmake_minimum_required(VERSION 3.12)

project("EyeCAN" VERSION 0.1)

# CONFIGURATION
set(CMAKE_CXX_STANDARD 17)

# Search for dbcppp in local installation
find_library(DBCPPP_LIBRARY NAMES dbcppp PATHS "$ENV{HOME}/.local/lib" NO_DEFAULT_PATH)
find_path(DBCPPP_INCLUDE_DIR dbcppp/Attribute.h PATHS "$ENV{HOME}/.local/include" NO_DEFAULT_PATH)

# If dbcppp was found, use it
if (DBCPPP_LIBRARY AND DBCPPP_INCLUDE_DIR)
    message(STATUS "Found dbcppp: ${DBCPPP_LIBRARY}")
    include_directories(${DBCPPP_INCLUDE_DIR})
else()
    message(WARNING "dbcppp not found. Using local dbcppp source files.")
    include_directories("dbcppp/include")
endif()

# Create executable
add_executable(EyeCAN "src/main.cpp")
target_include_directories(EyeCAN PUBLIC "include")

# Link dbcppp if found
if (DBCPPP_LIBRARY)
    target_link_libraries(EyeCAN ${DBCPPP_LIBRARY})
endif()
