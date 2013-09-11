# Display build information

macro(MACRO_BUILD_INFO)
    message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

    message(STATUS "Includes:")
    foreach(inc ${includes})
        message("     ${inc}")
    endforeach()

    message(STATUS "Libraries:")
    foreach(lib ${libraries})
        message("     ${lib}")
    endforeach()
endmacro()
