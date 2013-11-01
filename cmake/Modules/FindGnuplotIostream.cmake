# - Try to find Gnuplot-iostream
# Once done this will define
#  GNUPLOTIOSTREAM_FOUND - System has Gnuplot-iostream
#  GNUPLOTIOSTREAM_INCLUDE_DIR - The Gnuplot-iostream include directorie

find_path(GNUPLOT_IOSTREAM_INCLUDE_DIR NAMES gnuplot-iostream.h
          HINTS ENV GNUPLOT_IOSTREAM_DIR)

set(GNUPLOT_IOSTREAM_INCLUDE_DIRS ${GNUPLOT_IOSTREAM_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GnuplotIostream_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GnuplotIostream DEFAULT_MSG
                                  GNUPLOT_IOSTREAM_INCLUDE_DIR)

if(NOT GNUPLOTIOSTREAM_FOUND)
    message(STATUS "* Try to set GNUPLOT_IOSTREAM_DIR environment variable")
endif()

mark_as_advanced(GNUPLOT_IOSTREAM_INCLUDE_DIR)

