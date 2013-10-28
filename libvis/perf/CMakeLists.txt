add_definitions(-DENABLE_PERFTEST)
set(SRC_DIR ${libvis_SOURCE_DIR}/src)

macro(add_perf_test)
    include(MacroParseArguments)
    parse_arguments(PERF_TEST "NAME;SOURCES;LINK_LIBRARIES;ARGS" ${ARGN})
    list(GET PERF_TEST_DEFAULT_ARGS 0 PERF_TEST_NAME)

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

include_directories(${libvis_SOURCE_DIR}/test)

add_perf_test(NAME perf_index_build
    SOURCES perf_index_build.cpp
    LINK_LIBRARIES vis)

