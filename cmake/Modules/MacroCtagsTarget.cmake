# Adds the '[project]_tags' target.
# Usage:
#   add_ctags_target()
#   add_ctags_target(dependency)
macro(add_ctags_target)
    find_program(CTAGS ctags)
    set_source_files_properties(tags PROPERTIES GENERATED true)
    set(dependencies ${ARGN})
    message(STATUS "Will generate tags for project ${PROJECT_NAME}")
    add_custom_target(${PROJECT_NAME}_tags
        DEPENDS ${dependencies}
        COMMAND ${CTAGS} -R ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating CTAGS for ${PROJECT_NAME}")
endmacro()

# Shortcut for calling the 'tags' targets for all the given projects.
# Usage:
#   - in ProjectA/CMakeLists.txt:
#     add_ctags_target()
#   - in ProjectB/CMakeLists.txt:
#     add_ctags_target()
#   - in CMakeLists.txt:
#     add_ctags_for(projectA projectB)
macro(add_ctags_for)
    message(STATUS "Will generate tags for projects: ${ARGN}.")
    foreach(arg ${ARGN})
        list(APPEND dependencies ${arg}_tags)
    endforeach()
    add_custom_target(tags DEPENDS ${dependencies})
endmacro()

