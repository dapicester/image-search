/**
 * @file test_hog.cpp
 * @brief Test for function hogDescriptors
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE sanity
#include <boost/test/unit_test.hpp>

#include "hog.hpp"
#include "standardize.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(compute_hog) {
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
}

