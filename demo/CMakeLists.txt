project(demo)

message(STATUS "Building image-search demo")

set(LIBVIS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libvis/include")
list(APPEND includes ${LIBVIS_INCLUDE_DIR})
list(APPEND libraries vis)

find_package(Qt4 4.8 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})
list(APPEND libraries ${QT_LIBRARIES})

set(recompute_SRCS
    src/utils.cpp
    src/recompute.cpp
    src/recompute_main.cpp)

set(demo_SRCS
    src/demogui.cpp
    src/demogui_private.cpp
    src/demogui_slots.cpp
    src/demogui_images.cpp
    src/utils.cpp
    src/main.cpp)

set(demo_UIS
    src/demo.ui
    src/images.ui)

set(demo_MOCS
    src/demogui.hpp)

qt4_wrap_ui(demo_UIS_H ${demo_UIS})
qt4_wrap_cpp(demo_MOCS_SRCS ${demo_MOCS})

list(APPEND includes ${CMAKE_CURRENT_BINARY_DIR})

macro_build_info()

set(IMAGES_DIR ${CMAKE_SOURCE_DIR}/images)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
configure_file(src/directories.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/directories.hpp)
configure_file(src/config.yml.in ${CMAKE_CURRENT_BINARY_DIR}/config.yml)
message(STATUS "Test paths written to: ${CMAKE_CURRENT_BINARY_DIR}/directories.hpp")

include_directories(${includes})

add_executable(recompute ${recompute_SRCS})
target_link_libraries(recompute ${libraries})

add_executable(demo ${demo_SRCS} ${demo_UIS_H} ${demo_MOCS_SRCS})
target_link_libraries(demo ${libraries})

# remove warnings in Qt
foreach (target demo recompute)
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "-Wno-uninitialized")
endforeach()

if(ENABLE_CTAGS)
    add_ctags_target()
endif()

add_notes_target()

