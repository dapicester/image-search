# - Try to find yaml-cpp
# Once done this will define
#  YAML_CPP_FOUND - System has yaml-cpp
#  YAML_CPP_INCLUDE_DIRS - The yaml-cpp include directories
#  YAML_CPP_LIBRARIES - The libraries needed to use yaml-cpp
#  YAML_CPP_DEFINITIONS - Compiler switches required for using yaml-cpp

find_package(PkgConfig)
pkg_check_modules(PC_YAML_CPP QUIET yaml-cpp)

find_path(YAML_CPP_INCLUDE_DIR NAMES yaml.h
          PATH_SUFFIXES yaml-cpp)
find_library(YAML_CPP_LIBRARY NAMES yaml-cpp)

set(YAML_CPP_LIBRARIES ${YAML_CPP_LIBRARY} )
set(YAML_CPP_INCLUDE_DIRS ${YAML_CPP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set YAML_CPP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(yaml-cpp DEFAULT_MSG
                                  YAML_CPP_LIBRARY YAML_CPP_INCLUDE_DIR)

mark_as_advanced(YAML_CPP_INCLUDE_DIR YAML_CPP_LIBRARY )
