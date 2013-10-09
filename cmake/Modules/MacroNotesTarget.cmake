# Adds the 'notes' target displaying TODOs, FIXMEs, etc.
# Usage:
#   add_notes_target()
macro(add_notes_target)
    find_program(ACK NAMES ack ack-grep)
    if (ACK-NOTFOUND)
        set(command "grep -inRE 'todo|fixme|xxx' *")
    else()
        set(command "ack -i '(todo|fixme|xxx):?[[:space:]]+(.*)' --output=' $1 $2'")
    endif()
    add_custom_target(notes
        COMMAND sh -c ${command} && echo
        VERBATIM
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Showing notes")
endmacro()
