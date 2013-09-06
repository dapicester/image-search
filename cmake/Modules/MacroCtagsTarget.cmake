# Adds the 'tags' target.
# Usage:
#   add_ctags_target()
#   add_ctags_target(dependency)
macro(add_ctags_target)
    find_program(CTAGS ctags)
    set_source_files_properties(tags PROPERTIES GENERATED true)
    set(dependencies ${ARGN})
    add_custom_target(tags
        DEPENDS ${dependencies}
        COMMAND ${CTAGS} -R ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating CTAGS")
endmacro()
