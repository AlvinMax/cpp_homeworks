cmake_minimum_required(VERSION 3.13)
project(persistent_set)


include_directories(${persisnet_set_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(persistent_set main.cpp my_shared_ptr.h my_linked_ptr.h persistent_set.h)
