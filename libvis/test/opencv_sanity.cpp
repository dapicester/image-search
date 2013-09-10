/**
 * @file opencv_sanity.cpp
 * @brief Sanity test for the OpenCV libraries
 * @author Paolo D'Apice
 */

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "No image" << endl;
        return 1;
    }

    Mat image = imread(argv[1]);
    if (!image.data) {
        cout << "No image data" << endl;
        return 2;
    }

    if (argc > 2) {
        imshow("Display image", image);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }

    return 0;
}
