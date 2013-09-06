cmake_minimum_required(VERSION 2.8)
project(image-search)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(MacroBuildInfo)
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build("Project ${PROJECT_NAME} requires an out of source build.")
include(MacroAddUnitTest)

option(ENABLE_TESTING "Enables tests" ON)
if(ENABLE_TESTING)
    enable_testing()
endif()

option(BUILD_STATIC_LIBRARIES "Build static libraries" OFF)
if(BUILD_STATIC_LIBRARIES)
    message(STATUS "Building static libraries")
endif()

option(ENABLE_CTAGS "Enable CTags" OFF)
if(ENABLE_CTAGS)
    include(MacroCtagsTarget)
endif()

add_subdirectory(libvis)
add_subdirectory(demo)
