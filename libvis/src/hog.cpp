/**
 * @file hog.hpp
 * @brief HOG wrapper
 * @author Paolo D'Apice
 */

#include "hog.hpp"

using namespace cv;

namespace vis {

HogDescriptors::HogDescriptors(vl_size w, vl_size h, vl_size d)
        : width(w), height(h), dimension(d) {
    data = (float*) vl_malloc(width * height * dimension * sizeof(float));
}

HogDescriptors::~HogDescriptors() {
    vl_free(data);
}

Mat
HogDescriptors::toMat() const {
    Mat hog(dimension, width * height, CV_32F, data);
    Mat hogMatrix;
    flip(hog, hogMatrix, 1);
    return hogMatrix.clone();
}

HogExtractor::HogExtractor(int cs, int num) : cellSize(cs), numOrientations(num) {
    hog = vl_hog_new(VlHogVariantUoctti, numOrientations, VL_FALSE);
    vl_hog_set_use_bilinear_orientation_assignments(hog, VL_FALSE);
}

HogExtractor::~HogExtractor() {
    vl_hog_delete(hog);
}

HogDescriptors
HogExtractor::extract(const Mat& image) const {
    const Size& size = image.size();
    const float* ptr = image.ptr<float>(0);

    vl_hog_put_image(hog, ptr, size.width, size.height, image.channels(), cellSize);

    vl_size hogWidth = vl_hog_get_width(hog);
    vl_size hogHeight = vl_hog_get_height(hog);
    vl_size hogDimension = vl_hog_get_dimension(hog);

    HogDescriptors descriptors(hogWidth, hogHeight, hogDimension);
    vl_hog_extract(hog, descriptors.data);

    return descriptors;
}

Mat
HogExtractor::render(const HogDescriptors& descriptors) const {
    vl_size glyphSize = vl_hog_get_glyph_size(hog);
    vl_size imageHeight = glyphSize * descriptors.height;
    vl_size imageWidth = glyphSize * descriptors.width;

    float* image = (float*) vl_malloc(sizeof(float) * imageWidth * imageHeight);
    vl_hog_render(hog, image, descriptors.data, descriptors.width, descriptors.height);

    Mat hogImage = Mat(imageHeight, imageWidth, CV_32F, image).clone();
    vl_free(image);

    return hogImage;
}

} /* namespace vis */

