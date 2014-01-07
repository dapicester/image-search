add_definitions(-DENABLE_TESTING)

# new version of cmake overwrite instead of append, so let's backup
set(BOOST_LIBRARIES ${Boost_LIBRARIES})

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

# append the backupd ones
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARIES})

set(SRC ${server_SOURCE_DIR}/src)

message(STATUS "Tests:")

add_unit_test(NAME elpp_usage
    SOURCES elpp_usage.cpp
    LINK_LIBRARIES ${Boost_LIBRARIES})

add_unit_test(NAME test_configuration
    SOURCES test_configuration.cpp
            ${SRC}/configuration.cpp
    ARGS ${server_BINARY_DIR}/config.yml
    LINK_LIBRARIES ${YAML_CPP_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_protocol
    SOURCES test_protocol.cpp
            ${SRC}/protocol.cpp
    LINK_LIBRARIES ${Boost_LIBRARIES})

add_unit_test(NAME test_server
    SOURCES test_server.cpp
            ${SRC}/connection.cpp
            ${SRC}/connection_manager.cpp
            ${SRC}/logging.cpp
            ${SRC}/protocol.cpp
            ${SRC}/request_handler.cpp
            ${SRC}/server.cpp
    LINK_LIBRARIES vis ${Boost_LIBRARIES})
