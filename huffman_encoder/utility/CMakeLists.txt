cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_UTILITY)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

include_directories(${HUFFMAN_LIBRARY_SOURCE_DIR}/headers)
include_directories(${HUFFMAN_UTILITY_SOURCE_DIR}/headers)


set(HEADERS headers/file_reader.h
	    headers/file_writer.h
	    headers/compressor.h
	    headers/decompressor.h)

set(SOURCES file_reader.cpp
	    file_writer.cpp
	    compressor.cpp
	    decompressor.cpp
	    main.cpp)

add_executable(huffman ${HEADERS} ${SOURCES})
target_link_libraries(huffman huf_lib)
