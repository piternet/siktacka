cmake_minimum_required(VERSION 3.7)
project(client)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES client.cpp client.h)
find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(BoostTest ${SOURCE_FILES} )
target_link_libraries(BoostTest ${Boost_LIBRARIES})

find_package( Threads REQUIRED )
add_executable(client ${SOURCE_FILES})
target_link_libraries(client Threads::Threads)