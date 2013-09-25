# Adds the 'notes' target displaying TODOs, FIXMEs, etc.
# Usage:
#   add_notes_target()
macro(add_notes_target)
    add_custom_target(notes
        COMMAND find . | xargs grep --color=always -iRE "todo|fixme|xxx" | sort | uniq
        VERBATIM
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Showing notes")
endmacro()
