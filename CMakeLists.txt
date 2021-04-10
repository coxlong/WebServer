cmake_minimum_required(VERSION 3.0.0)
project(WebServer VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

include_directories (${PROJECT_SOURCE_DIR})

add_subdirectory(webserver)