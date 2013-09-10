add_unit_test(NAME opencv_sanity
    SOURCES opencv_sanity.cpp
    LINK_LIBRARIES ${OpenCV_LIBS}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)

add_unit_test(NAME vlfeat_sanity
    SOURCES vlfeat_sanity.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES})

add_unit_test(NAME test_kmeans
    SOURCES test_kmeans.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS})

add_unit_test(NAME test_standardize
    SOURCES test_standardize.cpp
    LINK_LIBRARIES ${VLFEAT_LIBRARIES} ${OpenCV_LIBS}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)
