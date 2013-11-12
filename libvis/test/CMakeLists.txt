add_definitions(-DENABLE_TESTING)
set(SRC_DIR ${libvis_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

# new version of cmake overwrite instead of append, so let's backup
set(BOOST_LIBRARIES ${Boost_LIBRARIES})

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
add_definitions(-DBOOST_TEST_DYN_LINK)

# append the backupd ones
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${BOOST_LIBRARIES})

message(STATUS "Tests:")

set(IMAGES_DIR ${CMAKE_SOURCE_DIR}/images)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)
configure_file(images.h.in ${CMAKE_CURRENT_BINARY_DIR}/images.h)
message(STATUS "Test paths written to: ${CMAKE_CURRENT_BINARY_DIR}/images.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_unit_test(NAME opencv_usage
    SOURCES opencv_usage.cpp
    LINK_LIBRARIES ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME armadillo_usage
    SOURCES armadillo_usage.cpp
    LINK_LIBRARIES ${ARMADILLO_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_conversions
    SOURCES test_conversions.cpp
    LINK_LIBRARIES ${ARMADILLO_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME vlfeat_usage
    SOURCES vlfeat_usage.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${Boost_LIBRARIES})

add_unit_test(NAME test_kmeans
    SOURCES test_kmeans.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_vl_kdtree
    SOURCES test_vl_kdtree.c
    DISABLED true
    LINK_LIBRARIES ${VLFEAT_LIBRARIES})

add_unit_test(NAME test_kdtree
    SOURCES test_kdtree.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_standardize
    SOURCES test_standardize.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_hog
    SOURCES test_hog.cpp
            ${SRC_DIR}/hog.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_hog_bow
    SOURCES test_hog_bow.cpp
            ${SRC_DIR}/hog.cpp
            ${SRC_DIR}/vocabulary.cpp
            ${SRC_DIR}/callbacks_hog.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_hsv_aux
    SOURCES test_hsv_aux.cpp
            ${SRC_DIR}/hsv_aux.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_hsv
    SOURCES test_hsv.cpp
            ${SRC_DIR}/hsv.cpp
            ${SRC_DIR}/hsv_aux.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_utils_ported
    SOURCES test_utils_ported.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_serialization
    SOURCES test_serialization.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_posixtimer
    SOURCES test_posixtimer.cpp
    DISABLED true
    LINK_LIBRARIES ${Boost_LIBRARIES})

add_unit_test(NAME test_timer
    SOURCES test_timer.cpp
    DISABLED true
    LINK_LIBRARIES ${Boost_LIBRARIES})

add_unit_test(NAME test_vocabulary
    SOURCES test_vocabulary.cpp
            ${SRC_DIR}/hog.cpp
            ${SRC_DIR}/vocabulary.cpp
            ${SRC_DIR}/callbacks_hog.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES})

add_unit_test(NAME test_descriptors
    SOURCES test_descriptors.cpp
    LINK_LIBRARIES vis ${Boost_LIBRARIES})

add_unit_test(NAME test_index
    SOURCES test_index.cpp
    LINK_LIBRARIES vis ${Boost_LIBRARIES})

#add_all_tests_target(${UNIT_TEST_TARGETS})

