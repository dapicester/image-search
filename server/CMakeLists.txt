project(server)

message(STATUS "Building image-search server")

set(LIBVIS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libvis/include")
set(EASYLOGGING_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/easylogging++")

list(APPEND includes ${LIBVIS_INCLUDE_DIR} ${EASYLOGGING_INCLUDE_DIR})
list(APPEND libraries vis)

list(APPEND includes "${server_SOURCE_DIR}/include")

find_package(Boost COMPONENTS iostreams serialization thread REQUIRED)
list(APPEND includes ${Boost_INCLUDE_DIRS})
list(APPEND libraries ${Boost_LIBRARIES})

set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
configure_file("include/directories.h.in" ${CMAKE_CURRENT_BINARY_DIR}/directories.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(server_SRCS
    src/connection.cpp
    src/connection_manager.cpp
    src/logging.cpp
    src/protocol.cpp
    src/main.cpp
    src/request_handler.cpp
    src/server.cpp
)

list(APPEND includes ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${includes})

macro_build_info()

add_executable(server ${server_SRCS})
target_link_libraries(server ${libraries})

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

if(ENABLE_CTAGS)
    add_ctags_target()
endif()

add_notes_target()

