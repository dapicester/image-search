cmake_minimum_required(VERSION 2.8)
project(libvis)

set(libvis_VERSION_MAJOR 0)
set(libvis_VERSION_MINOR 1)
set(libvis_VERSION_PATCH 0)

message(STATUS "Building libvis")
macro_build_info()

list(APPEND includes ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV COMPONENTS core imgproc highgui REQUIRED)
list(APPEND libraries ${OpenCV_LIBS})
list(APPEND includes ${OpenCV_INCLUDE_DIRS})

find_package(VLFeat REQUIRED)
list(APPEND libraries ${VLFEAT_LIBRARIES})
list(APPEND includes ${VLFEAT_INCLUDE_DIRS})

file(GLOB imsearch_SRCS src/*.c src/*.cpp)

list(REMOVE_DUPLICATES includes)
include_directories(${includes})

if(BUILD_STATIC_LIBRARIES)
    add_library(vis STATIC ${imsearch_SRCS})
else()
    add_library(vis SHARED ${imsearch_SRCS})
endif()
target_link_libraries(vis ${libraries})

if(ENABLE_CTAGS)
    add_ctags_target(vis)
endif()

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

