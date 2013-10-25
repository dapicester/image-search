# Adds the 'notes' target displaying TODOs, FIXMEs, etc.
# Usage:
#   add_notes_target()
macro(add_notes_target)
    find_program(ACK NAMES ack ack-grep)
    if (ACK-NOTFOUND)
        set(command "grep -inRE 'todo|fixme|xxx' ${CMAKE_CURRENT_SOURCE_DIR}/*")
    else()
        set(command "${ACK} -i '(todo|fixme|xxx):?[[:space:]]+(.*)' --nopager --output=' $1 $2' ${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
    message(STATUS "Will generate notes for project ${PROJECT_NAME}")
    add_custom_target(${PROJECT_NAME}_notes
        COMMAND sh -c ${command} && echo
        VERBATIM
        COMMENT "Showing notes")
endmacro()

# Shortcut for calling the 'notes' targets for all the given projects.
# Usage:
#   - in ProjectA/CMakeLists.txt:
#     add_notes_target()
#   - in ProjectB/CMakeLists.txt:
#     add_notes_target()
#   - in CMakeLists.txt:
#     add_notes_for(projectA projectB)
macro(add_notes_for)
    message(STATUS "Will generate notes for projects: ${ARGN}.")
    foreach(arg ${ARGN})
        list(APPEND dependencies ${arg}_notes)
    endforeach()
    add_custom_target(notes DEPENDS ${dependencies})
endmacro()

