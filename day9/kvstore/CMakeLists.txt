cmake_minimum_required(VERSION 3.16)
project(kvstore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQURIED ON)

add_library(kvstore src/kv_store.cpp)

target_include_directories(kvstore PUBLIC include)

add_executable(example examples/basic.cpp)

target_link_libraries(example kvstore)