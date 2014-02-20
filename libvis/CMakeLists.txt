cmake_minimum_required(VERSION 2.8)
project(libvis)

message(STATUS "Building libvis")

#list(APPEND includes ${CMAKE_SOURCE_DIR}/easylogging++)
list(APPEND includes ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV COMPONENTS core imgproc highgui REQUIRED)
list(APPEND libraries ${OpenCV_LIBS})
list(APPEND includes ${OpenCV_INCLUDE_DIRS})

find_package(BLAS)
if(BLAS_FOUND)
    message(STATUS "BLAS found: ${BLAS_LIBRARIES}")
    set(MATH_LIBRARIES ${BLAS_LIBRARIES})
    add_definitions(-DARMA_DONT_USE_WRAPPER)
endif()

find_package(Armadillo REQUIRED)
list(APPEND includes ${ARMADILLO_INCLUDE_DIRS})
if(NOT BLAS_FOUND)
    message(STATUS "BLAS not found, linking to Armadillo")
    set(MATH_LIBRARIES ${ARMADILLO_LIBRARIES})
endif()

list(APPEND libraries ${MATH_LIBRARIES})

find_package(VLFeat REQUIRED)
list(APPEND libraries ${VLFEAT_LIBRARIES})
list(APPEND includes ${VLFEAT_INCLUDE_DIRS})

find_package(Boost COMPONENTS filesystem iostreams serialization system thread REQUIRED)
list(APPEND includes ${Boost_INCLUDE_DIRS})
list(APPEND libraries ${Boost_LIBRARIES})

find_package(YamlCpp REQUIRED)
list(APPEND includes ${YAML_CPP_INCLUDE_DIRS})
list(APPEND libraries ${YAML_CPP_LIBRARIES})

set(imsearch_SRCS
    src/builder.cpp
    src/callbacks.cpp
    src/configuration.cpp
    src/descriptors.cpp
    src/hog.cpp
    src/hsv_aux.cpp
    src/hsv.cpp
    src/imsearch.cpp
    src/index.cpp
    src/vocabulary.cpp
    src/utils/filesystem.cpp
    src/utils/thread_pool.cpp
)

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

