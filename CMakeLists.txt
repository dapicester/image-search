cmake_minimum_required(VERSION 2.8)
project(image-search)

set(image-search_VERSION_MAJOR 0)
set(image-search_VERSION_MINOR 1)
set(image-search_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(EnableCXX11)
include(MacroBuildInfo)
include(MacroOutOfSourceBuild)
macro_ensure_out_of_source_build("Project ${PROJECT_NAME} requires an out of source build.")
include(MacroAddUnitTest)
include(MacroCtagsTarget)
include(MacroNotesTarget)

option(ENABLE_TESTING "Enables tests" ON)
if(ENABLE_TESTING)
    enable_testing()
endif()

option(ENABLE_PERFTEST "Enables performance test" ON)

option(BUILD_STATIC_LIBRARIES "Build static libraries" OFF)
if(BUILD_STATIC_LIBRARIES)
    message(STATUS "Building static libraries")
endif()

option(ENABLE_CTAGS "Enable CTags" OFF)

add_subdirectory(libvis)
add_subdirectory(demo)
add_subdirectory(server)

if(ENABLE_CTAGS)
    add_ctags_for(libvis demo server)
endif()

add_notes_for(libvis demo server)

