cmake_minimum_required(VERSION 3.7)
project(klient)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES client.cpp client.h)
add_executable(klient ${SOURCE_FILES})