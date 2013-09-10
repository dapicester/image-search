/**
 * @file test_standardize.cpp
 * @brief Test for function standardizeImage
 * @author Paolo D'Apice
 */

#include "standardize.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    Mat image = imread(argv[1]);
    Mat standard;

    standardizeImage(image, standard, 128);
    assert(standard.depth() == CV_32F);
    assert(standard.size().height == 128);

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }

    return 0;
}

