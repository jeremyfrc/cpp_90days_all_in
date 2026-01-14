cmake_minimum_required(VERSION 3.16)
project(filelogger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED ON)

add_library(filelogger src/file_logger.cpp)

target_include_directories(filelogger PUBLIC include)

add_executable(example1 examples/basic.cpp)
add_executable(example2 examples/advanced.cpp)

target_link_libraries(example1 filelogger)
target_link_libraries(example2 filelogger)