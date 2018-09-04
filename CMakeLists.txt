cmake_minimum_required(VERSION 3.10)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)




find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(untitled1 main.cpp Robot.cpp Robot.h defines.h)


target_link_libraries(untitled1 ${SDL2_LIBRARIES})