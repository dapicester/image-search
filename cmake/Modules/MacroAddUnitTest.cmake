# usage: add_unit_test(NAME name
#                      SOURCES source1 source2 ...
#                      [DISABLED true]
#                      [LINK_LIBRARIES lib1 lib2 ...]
#                      [WORKING_DIRECTORY dir]
#                      [ARGS arg1 arg2 ...]
#                      [TAG tag])
# It makes available the variable UNIT_TEST_TARGETS containing all the unit tests
macro(add_unit_test)
    set(options DISABLED)
    set(oneValueArgs NAME WORKING_DIRECTORY TAG)
    set(multiValueArgs SOURCES COMPILE_FLAGS LINK_LIBRARIES ARGS)
    cmake_parse_arguments(UNIT_TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT UNIT_TEST_NAME)
        message(FATAL_ERROR "must specify the test name")
    endif()

    add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
    if(UNIT_TEST_LINK_LIBRARIES)
        target_link_libraries(${UNIT_TEST_NAME} ${UNIT_TEST_LINK_LIBRARIES})
    endif()

    if(UNIT_TEST_COMPILE_FLAGS)
        set_target_properties(${UNIT_TEST_NAME} PROPERTIES COMPILE_FLAGS ${UNIT_TEST_COMPILE_FLAGS})
    endif()

    if(UNIT_TEST_TAG)
        set(testName "${UNIT_TEST_TAG}/${UNIT_TEST_NAME}")
    else()
        set(testName "${UNIT_TEST_NAME}")
    endif()

    if (UNIT_TEST_DISABLED)
        message(STATUS "* skipped test: ${testName}")
    else()
        add_test(NAME ${testName}
                 WORKING_DIRECTORY ${UNIT_TEST_WORKING_DIRECTORY}
                 COMMAND ${UNIT_TEST_NAME} ${UNIT_TEST_ARGS})
        list(APPEND UNIT_TEST_TARGETS  ${testName})
        message(STATUS "  added test: ${testName}")
    endif()
endmacro()

# usage: add_all_tests_target(tests)
macro(add_all_tests_target tests)
    add_custom_target(all_tests ALL DEPENDS ${tests})
    add_custom_command(TARGET all_tests
                       COMMENT "Run all tests"
                       POST_BUILD
                       COMMAND ctest ARGS --output-on-failure
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    message(STATUS "Enabled testing on build")
endmacro()
