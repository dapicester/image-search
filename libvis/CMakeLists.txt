cmake_minimum_required(VERSION 2.8)
project(libvis)

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(imsearch_SRCS
    src/imsearch.cpp
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_INCLUDE_DIR}")

add_library(vis SHARED ${imsearch_SRCS})
#add_library(vis-static STATIC ${imsearch_SRCS})

