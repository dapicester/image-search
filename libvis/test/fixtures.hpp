/**
 * @file fixtures.hpp
 * @brief Test fixtures loading images.
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_FIXTURES_HPP
#define VIS_TEST_FIXTURES_HPP

#include "images.h"

#include "vis/standardize.hpp"
#include "vis/utils/filesystem.hpp"

#include <opencv2/highgui/highgui.hpp>

namespace test {

namespace fs = boost::filesystem;

/// Full path to the image dir in the project root directory.
static const fs::path imageDir(IMAGES_DIR);

static const std::string LENA = (imageDir / "lena.png").string();       ///< Full path to lena.png
static const std::string PEPPERS = (imageDir / "peppers.png").string(); ///< Full path to peppers.png

/**
 * @brief Load and standardize an image.
 * @param[in] name Full path to image file.
 * @param[out] image Image data.
 * @param[out] standardized Standardized image data.
 */
void
loadImage(const std::string& name, cv::Mat& image, cv::Mat& standardized) {
    image = cv::imread(name);
    BOOST_REQUIRE_MESSAGE(image.data, "Require image");
    vis::standardizeImage(image, standardized, 128);
}

/**
 * @brief Load file names from a directory.
 * @param[in] dirname Full path to directory.
 * @param[out] names List of files in \a dirname.
 */
void
loadFiles(const std::string& dirname, std::vector<fs::path>& names) {
    const fs::path dir(dirname);
    BOOST_REQUIRE_MESSAGE(fs::is_directory(dir), "invalid image dir");

    names = vis::getImageFiles(dir);

    // arrange as matlab
    std::sort(names.rbegin(), names.rend());
}

/// @brief Base class for fixtures loading one image.
struct ImageFixture {
    cv::Mat input; ///< Original image.
    cv::Mat image; ///< Standardized image.
};

/// @brief Base class for fixtures loading files from a directory.
struct DirFixture {
    std::vector<fs::path> files;
};

/// @brief Test fixture loading the lena image.
struct Lena : ImageFixture {
    Lena() {
        loadImage(LENA, input, image);
    }
};

/// @brief Test fixture loading the peppers image.
struct Peppers : ImageFixture {
    Peppers() {
        loadImage(PEPPERS, input, image);
    }
};

/// @brief Test fixtures loading names of files in \a IMAGES_DIR.
struct ImageDir : DirFixture {
    ImageDir() {
        loadFiles(IMAGES_DIR, files);
    }
};

} /* namespace test */

#endif

