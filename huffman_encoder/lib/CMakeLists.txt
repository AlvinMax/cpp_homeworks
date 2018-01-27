cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_LIBRARY)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

include_directories(${HUFFMAN_LIBRARY_SOURCE_DIR}/headers)

set(LIB_HEADERS headers/summator.h
                headers/consts.h
                headers/code.h
                headers/word.h
                headers/huffman_tree.h
                headers/encoder.h
                headers/decoder.h
        )

set(LIB_SOURCES summator.cpp
                word.cpp
                huffman_tree.cpp
                encoder.cpp
                decoder.cpp
        )

add_library(huf_lib ${LIB_HEADERS} ${LIB_SOURCES})
