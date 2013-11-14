/**
 * @file hog.cpp
 * @brief VlFeat HOG wrapper
 * @author Paolo D'Apice
 */

#include "hog.hpp"
#include <memory>

namespace vis {

HogExtractor::HogExtractor(int cs, int no) :
        cellSize(cs), numOrientations(no) {
    hog = vl_hog_new(VlHogVariantUoctti, numOrientations, VL_FALSE);
    vl_hog_set_use_bilinear_orientation_assignments(hog, VL_FALSE);
}

HogExtractor::~HogExtractor() {
    vl_hog_delete(hog);
}

arma::fmat
HogExtractor::extract(const cv::Mat& image) const {
    vl_hog_put_image(hog,
            image.ptr<float>(0),
            image.cols, image.rows, image.channels(),
            cellSize);

    vl_size hogWidth = vl_hog_get_width(hog);
    vl_size hogHeight = vl_hog_get_height(hog);
    vl_size hogDimension = vl_hog_get_dimension(hog);

    arma::fmat descriptors(hogWidth * hogHeight, hogDimension);
    vl_hog_extract(hog, descriptors.memptr());

    return arma::fliplr(descriptors.t());
}

cv::Mat
HogExtractor::render(const arma::fmat& descriptors) const {
    vl_size glyphSize = vl_hog_get_glyph_size(hog);
    vl_size imageHeight = glyphSize * vl_hog_get_height(hog);
    vl_size imageWidth = glyphSize * vl_hog_get_width(hog);

    std::unique_ptr<float> image(new float[imageWidth * imageHeight]);
    vl_hog_render(hog, image.get(), descriptors.memptr(), vl_hog_get_height(hog), vl_hog_get_width(hog));

    return cv::Mat(imageHeight, imageWidth, CV_32F, image.get()).clone();
}

} /* namespace vis */

