cmake_minimum_required(VERSION 3.5)
set(CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++14)
add_definitions(-Werror)

project(Sorbet)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_definitions(
    -DSOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Utils)
add_subdirectory(Compiler)
add_subdirectory(ParseLib)
add_subdirectory(Parser)
