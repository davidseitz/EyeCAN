cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 23)

add_executable(EyeCAN main.cpp
        src/header/eyecanserver.h
        src/source/eyecanserver.cpp)
