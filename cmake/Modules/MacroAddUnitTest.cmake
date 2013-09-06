# usage: add_unit_test(NAME name 
#                      SOURCES source1 source2 ...
#                      [LINK_LIBRARIES lib1 lib2 ...]
#                      [TAG tag])
include(MacroParseArguments)
macro(add_unit_test)
    parse_arguments(UNIT_TEST "NAME;SOURCES;LINK_LIBRARIES;TAG" ${ARGN})
    list(GET UNIT_TEST_DEFAULT_ARGS 0 UNIT_TEST_NAME)

    if(NOT UNIT_TEST_NAME)
        message(FATAL_ERROR "must specify the test name")
    endif()

    add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
    if(UNIT_TEST_LINK_LIBRARIES)
        target_link_libraries(${UNIT_TEST_NAME} ${UNIT_TEST_LINK_LIBRARIES})
    endif()
    if(UNIT_TEST_TAG)
        set(testName "${UNIT_TEST_TAG}/${UNIT_TEST_NAME}")
    else()
        set(testName "${UNIT_TEST_NAME}")
    endif()
    add_test(NAME ${testName} COMMAND ${UNIT_TEST_NAME})
    message(STATUS "  added test: ${testName}")
endmacro()
