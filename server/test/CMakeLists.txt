add_definitions(-DENABLE_TESTING)

# new version of cmake overwrite instead of append, so let's backup
set(BOOST_LIBRARIES ${Boost_LIBRARIES})

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

# append the backupd ones
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARIES})

set(SRC ${server_SOURCE_DIR}/src)

message(STATUS "Tests:")

add_unit_test(NAME test_protocol
    SOURCES test_protocol.cpp
    LINK_LIBRARIES ${Boost_LIBRARIES})

add_unit_test(NAME test_server
    SOURCES test_server.cpp
            ${SRC}/connection.cpp
            ${SRC}/server.cpp
    LINK_LIBRARIES ${Boost_LIBRARIES})
