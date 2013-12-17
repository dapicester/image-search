add_definitions(-DENABLE_PERFTEST)
set(SRC_DIR ${libvis_SOURCE_DIR}/src)

macro(add_perf_test)
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LINK_LIBRARIES ARGS)
    cmake_parse_arguments(PERF_TEST "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT PERF_TEST_NAME)
        message(FATAL_ERROR "must specify the test name")
    endif()

    add_executable(${PERF_TEST_NAME} ${PERF_TEST_SOURCES})
    if(PERF_TEST_LINK_LIBRARIES)
        target_link_libraries(${PERF_TEST_NAME} ${PERF_TEST_LINK_LIBRARIES})
    endif()

    message(STATUS "  added performance test: ${PERF_TEST_NAME}")
endmacro()

message(STATUS "Performance tests:")

set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
set(PLOT_DIR ${libvis_SOURCE_DIR}/perf)
configure_file(dir.h.in ${CMAKE_CURRENT_BINARY_DIR}/dir.h)
message(STATUS "Paths written to: ${CMAKE_CURRENT_BINARY_DIR}/dir.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_perf_test(NAME perf_index_build
    SOURCES perf_index_build.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_index_build_numtrees
    SOURCES perf_index_build_numtrees.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_index_query
    SOURCES perf_index_query.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_index_query_numtrees
    SOURCES perf_index_query_numtrees.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_index_query_neighbors
    SOURCES perf_index_query_neighbors.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_index_query_ann
    SOURCES perf_index_query_ann.cpp perf_index.cpp
    LINK_LIBRARIES vis)

add_perf_test(NAME perf_descriptors
    SOURCES perf_descriptors.cpp
            ${SRC_DIR}/utils/filesystem.cpp
    LINK_LIBRARIES vis)

add_executable(perf_results perf_results.cpp)
target_link_libraries(perf_results ${Boost_LIBRARIES})
message(STATUS "  added performance results tool: perf_results")

find_package(Gnuplot)
if (GNUPLOT_FOUND)
    message(STATUS "Gnuplot found, checking for Gnuplot-iostream ...")
    find_package(GnuplotIostream)
    if (GNUPLOTIOSTREAM_FOUND)
        message(STATUS "* enabled graphs generation")
        include_directories(${GNUPLOT_IOSTREAM_INCLUDE_DIRS})
        set_target_properties(perf_results PROPERTIES COMPILE_FLAGS "-DHAVE_GNUPLOT_IOSTREAM")
    else()
        message(STATUS "* not generating graphs")
    endif()
endif()

