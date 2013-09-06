add_unit_test(NAME opencv_sanity
    SOURCES opencv_sanity.cpp
    LINK_LIBRARIES ${OpenCV_LIBS}
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/lena.png)

