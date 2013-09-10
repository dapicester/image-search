/**
 * @file test_kmeans.cpp
 * @brief K-means test
 * @author Paolo D'Apice
 */

#include "kmeans.hpp"

using namespace cv;

Mat getTestData(int dimension, int numData) {
    VlRand rand;
    vl_rand_init(&rand);
    vl_rand_seed(&rand, 1000);

    Mat data(dimension, numData, CV_32F);
    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < numData; j++) {
            float randomNum = (float) vl_rand_real3(&rand) + 1;
            data.at<float>(i,j) = randomNum;
        }
    }
    return data;
}

int main(int, char**) {
    Mat data = getTestData(256, 10000);
    VlKMeans* centers = vis::kmeans(data, 40);
    vl_kmeans_delete(centers);
    return 0;
}

