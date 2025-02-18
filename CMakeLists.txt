cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_SOURCE_DIR}/libaries/json/include)
include_directories(${CMAKE_SOURCE_DIR}/libaries/cpp-httplib)
include_directories(${CMAKE_SOURCE_DIR}/src/header)
include_directories(${CMAKE_SOURCE_DIR}/src/source)


add_executable(EyeCAN main.cpp
        src/header/eyecanserver.h
        src/source/eyecanserver.cpp)

target_link_libraries(EyeCAN ws2_32)
