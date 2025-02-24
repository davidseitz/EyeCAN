cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/libraries/json/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/cpp-httplib)
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(EyeCAN main.cpp
        include/eyecanserver.h
        source/eyecanserver.cpp)

target_link_libraries(EyeCAN ws2_32)