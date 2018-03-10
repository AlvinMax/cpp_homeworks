cmake_minimum_required(VERSION 3.8)
project(debug_list)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp debug_list.h)
#add_executable(debug_list ${SOURCE_FILES} )
add_executable(main main.cpp debug_list.h)