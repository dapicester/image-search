add_definitions(-DENABLE_TESTING)

# new version of cmake overwrite instead of append, so let's backup
set(BOOST_LIBRARIES ${Boost_LIBRARIES})

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

# append the backupd ones
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARIES})

set(SRC ${server_SOURCE_DIR}/src)

message(STATUS "Tests:")

add_unit_test(NAME msgpack_usage
    SOURCES msgpack_usage.cpp
    LINK_LIBRARIES ${MSGPACK_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_protocol
    SOURCES test_protocol.cpp
    LINK_LIBRARIES ${MSGPACK_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_server
    SOURCES test_server.cpp
            ${SRC}/connection.cpp
            ${SRC}/connection_manager.cpp
            ${SRC}/request_handler.cpp
            ${SRC}/server.cpp
    ARGS ${server_BINARY_DIR}/config.yml
    LINK_LIBRARIES vis ${Boost_LIBRARIES})
