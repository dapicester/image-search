# - Try to find VLFeat
# Once done this will define
#  VLFEAT_FOUND - System has VLFeat
#  VLFEAT_INCLUDE_DIRS - The VLFeat include directories
#  VLFEAT_LIBRARIES - The libraries needed to use VLFeat
#  VLFEAT_DEFINITIONS - Compiler switches required for using VLFeat

find_path(VLFEAT_INCLUDE_DIR NAMES vl/generic.h
          PATH_SUFFIXES vl)
find_library(VLFEAT_LIBRARY NAMES vl libvl)

set(VLFEAT_LIBRARIES ${VLFEAT_LIBRARY} )
set(VLFEAT_INCLUDE_DIRS ${VLFEAT_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set VLFEAT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(VLFeat DEFAULT_MSG
                                  VLFEAT_LIBRARY VLFEAT_INCLUDE_DIR)

mark_as_advanced(VLFEAT_INCLUDE_DIR VLFEAT_LIBRARY )
