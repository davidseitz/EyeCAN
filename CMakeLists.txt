cmake_minimum_required(VERSION 3.12)

project("EyeCAN" VERSION 0.1)

# CONFIGURATION

set(CMAKE_CXX_STANDARD 17)

find_package(dbcppp)

if (NOT dbcppp_FOUND)
    message(WARNING "dbcppp not found. Using libEyeCAN dbcppp (dbcppp)")
    include_directories("dbcppp")
endif()
add_executable(EyeCAN "src/main.cpp")
target_include_directories(EyeCAN PUBLIC "include")
target_include_directories(EyeCAN PUBLIC "dbcppp/include")
target_link_libraries(EyeCAN dbcppp)