cmake_minimum_required(VERSION 3.6)
project(c___teste)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    client.cpp
    helloworld.cpp
    refereng.cpp
    server.cpp)

add_executable(c___teste ${SOURCE_FILES})