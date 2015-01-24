# - Try to find Msgpack
# Once done this will define
#  MSGPACK_FOUND - System has Msgpack
#  MSGPACK_INCLUDE_DIRS - The Msgpack include directories
#  MSGPACK_LIBRARIES - The libraries needed to use Msgpack
#  MSGPACK_DEFINITIONS - Compiler switches required for using Msgpack

find_path(MSGPACK_INCLUDE_DIR NAMES msgpack.hpp)
find_library(MSGPACK_LIBRARY NAMES msgpack libmsgpack)

set(MSGPACK_LIBRARIES ${MSGPACK_LIBRARY})
set(MSGPACK_INCLUDE_DIRS ${MSGPACK_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set MSGPACK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Msgpack DEFAULT_MSG
                                  MSGPACK_LIBRARY MSGPACK_INCLUDE_DIR)

mark_as_advanced(MSGPACK_INCLUDE_DIR MSGPACK_LIBRARY)
