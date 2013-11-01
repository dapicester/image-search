# Display build information

macro(MACRO_BUILD_INFO)
    set(options INCLUDES LIBRARIES)
    cmake_parse_arguments(BUILD_INFO "${options}" "" "" ${ARGN})

    if (CMAKE_BUILD_TYPE)
        message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
    else()
        message(STATUS "Build Type: <NOT DEFINED>")
    endif()

    if (BUILD_INFO_INCLUDES)
        message(STATUS "Includes:")
        foreach(inc ${includes})
            message("     ${inc}")
        endforeach()
    endif()

    if (BUILD_INFO_LIBRARIES)
        message(STATUS "Libraries:")
        foreach(lib ${libraries})
            message("     ${lib}")
        endforeach()
    endif()
endmacro()
