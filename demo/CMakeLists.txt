project(demo)

message(STATUS "Building image-search demo")

set(LIBVIS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libvis/include")
list(APPEND includes ${LIBVIS_INCLUDE_DIR})
list(APPEND libraries vis)

find_package(Qt4 4.8 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})
list(APPEND libraries ${QT_LIBRARIES})

set(demo_SRCS
    src/demogui.cpp
    src/demogui_private.cpp
    src/demogui_slots.cpp
    src/utils.cpp
    src/main.cpp)

set(demo_UIS src/demo.ui)
qt4_wrap_ui(demo_UIS_H ${demo_UIS})

set(demo_MOCS src/demogui.hpp)
qt4_wrap_cpp(demo_MOCS_SRCS ${demo_MOCS})

list(APPEND includes ${CMAKE_CURRENT_BINARY_DIR})

macro_build_info()

set(IMAGES_DIR ${CMAKE_SOURCE_DIR}/images)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
configure_file(src/directories.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/directories.hpp)
message(STATUS "Test paths written to: ${CMAKE_CURRENT_BINARY_DIR}/directories.hpp")

include_directories(${includes})
add_executable(demo ${demo_SRCS} ${demo_UIS_H} ${demo_MOCS_SRCS})
target_link_libraries(demo ${libraries})

set_target_properties(demo PROPERTIES COMPILE_FLAGS "-Wno-uninitialized")

#add_notes_target() FIXME conflicts with target in libvis

