cmake_minimum_required(VERSION 2.8)
project(libvis)

set(libvis_VERSION_MAJOR 0)
set(libvis_VERSION_MINOR 1)
set(libvis_VERSION_PATCH 0)

message(STATUS "Building libvis")

list(APPEND includes ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV COMPONENTS core imgproc highgui REQUIRED)
list(APPEND libraries ${OpenCV_LIBS})
list(APPEND includes ${OpenCV_INCLUDE_DIRS})

find_package(VLFeat REQUIRED)
list(APPEND libraries ${VLFEAT_LIBRARIES})
list(APPEND includes ${VLFEAT_INCLUDE_DIRS})

find_package(Boost COMPONENTS filesystem iostreams serialization system REQUIRED)
list(APPEND includes ${Boost_INCLUDE_DIRS})
list(APPEND libraries ${Boost_LIBRARIES})

file(GLOB imsearch_SRCS src/*.c src/*.cpp)

list(REMOVE_DUPLICATES includes)
include_directories(${includes})

macro_build_info()

if(BUILD_STATIC_LIBRARIES)
    add_library(vis STATIC ${imsearch_SRCS})
else()
    add_library(vis SHARED ${imsearch_SRCS})
endif()
target_link_libraries(vis ${libraries})

if(ENABLE_CTAGS)
    add_ctags_target()
endif()

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

if (ENABLE_PERFTEST)
    add_subdirectory(perf)
endif()

add_notes_target()

