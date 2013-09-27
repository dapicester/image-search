/**
 * @file test_serialization.cpp
 * @brief Test for data serialization
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE serialization
#include <boost/test/unit_test.hpp>

#include "utils/matrix.hpp"
#include "serialization.hpp"
#include <opencv2/core/core.hpp>
#include <fstream>
#include <iostream>

using cv::Mat;
using std::cout;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;

using namespace vis;

template <typename InputArchive, typename OutputArchive>
void doTest(const Mat& input, const char* filename, bool compression) {
    if (compression) {
        static compress_serializer<OutputArchive, Mat, io::gzip_compressor> GzipSaver;
        GzipSaver(filename, input);
    }
    else {
        static serializer<OutputArchive, Mat> Saver;
        Saver(filename, input);
    }

    Mat loaded;
    if (compression) {
        static compress_deserializer<InputArchive, Mat, io::gzip_decompressor> GzipLoader;
        GzipLoader(filename, loaded);
    }
    else {
        static deserializer<InputArchive, Mat> Loader;
        Loader(filename, loaded);
    }

    BOOST_CHECK(equals(loaded, input));
}

BOOST_AUTO_TEST_CASE(matrix_serialization) {
    Mat mat(4, 4, CV_32F);
    cv::randu(mat, cv::Scalar(0), cv::Scalar(255));

    // plain text
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "matrix.txt", false);
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "matrix.txt.gz", true);

    // binary
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "matrix.bin", false);
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "matrix.bin.gz", true);
}

