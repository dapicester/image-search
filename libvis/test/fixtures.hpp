/**
 * @file fixtures.hpp
 * @brief Test fixtures loading images.
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_FIXTURES_HPP
#define VIS_TEST_FIXTURES_HPP

#include "images.hpp"
#include "standardize.hpp"
#include <opencv2/highgui/highgui.hpp>

/// @brief Load and standardize an image.
void
loadImage(const std::string& name, cv::Mat& image, cv::Mat& standardized) {
    image = cv::imread(name);
    BOOST_REQUIRE_MESSAGE(image.data, "Require image");
    vis::standardizeImage(image, standardized, 128);
}

struct ImageFixture {
    cv::Mat input; ///< Original image.
    cv::Mat image; ///< Standardized image.
};

struct Lena : ImageFixture {
    Lena() {
        loadImage(LENA, input, image);
    }
};

struct Peppers : ImageFixture {
    Peppers() {
        loadImage(PEPPERS, input, image);
    }
};

#endif

