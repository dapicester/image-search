/**
 * @file test_hog.cpp
 * @brief Test for function hogDescriptors
 * @author Paolo D'Apice
 */

#include "hog.hpp"
#include "standardize.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat input = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    Mat image;
    standardizeImage(input, image, 256);

    HogExtractor extractor;
    HogDescriptors descriptors = extractor.extract(image);
    Mat hogMatrix = descriptors.toMat();

    //Size size = extractor.size();
    //std::cout << size << std::endl;
    // TODO assertions with boost test
    //assert(size == Size(31, 4096));

    if (argc > 2) {
        namedWindow("image");
        imshow("image", input);

        Mat hogImage = extractor.render(descriptors);
        namedWindow("hog");
        imshow("hog", hogImage);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
    return 0;
}

