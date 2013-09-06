cmake_minimum_required(VERSION 2.8)
project(demo)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(demo_SRCS
    src/main.cpp
)

set(LIBVIS_INCLUDE_DIR "../libvis/include")

include_directories(${LIBVIS_INCLUDE_DIR})
add_executable(demo ${demo_SRCS})
target_link_libraries(demo vis)

