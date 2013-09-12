set(SRC_DIR ${libvis_SOURCE_DIR}/src)

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

message(STATUS "Tests:")

add_unit_test(NAME opencv_sanity
    SOURCES opencv_sanity.cpp
    LINK_LIBRARIES ${OpenCV_LIBS} ${Boost_LIBRARIES}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)

add_unit_test(NAME vlfeat_sanity
    SOURCES vlfeat_sanity.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_kmeans
    SOURCES test_kmeans.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_standardize
    SOURCES test_standardize.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)

add_unit_test(NAME test_hog
    SOURCES test_hog.cpp ${SRC_DIR}/hog.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)

add_unit_test(NAME test_linspace
    SOURCES test_linspace.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_utils_ported
    SOURCES test_utils_ported.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

