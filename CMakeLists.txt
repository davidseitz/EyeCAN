cmake_minimum_required(VERSION 3.30)
project(EyeCAN)

set(CMAKE_CXX_STANDARD 23)

#Add the libxml2 library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake/")
find_package(LibXml2 REQUIRED)

#Add the dbcppp library
add_subdirectory("dbcppp")
target_include_directories(dbcppp PUBLIC "dbcppp/include")


add_executable(EyeCAN main.cpp)

# Link the dbcppp and libxml2 libraries
target_link_libraries(EyeCAN dbcppp xml2)