/**
 * @file test_serialization.cpp
 * @brief Test for data serialization
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE serialization
#include <boost/test/unit_test.hpp>

#include "utils/matrix.hpp"
#include "utils/serialization.hpp"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <opencv2/core/core.hpp>
#include <fstream>
#include <iostream>

using cv::Mat;
using std::cout;
using std::endl;
using std::ios;
using std::ifstream;
using std::ofstream;

// TODO move to serialization.hpp
template <typename Archive, typename Object>
void save(const char* file, const Object& obj) {
    ofstream os(file, ios::out | ios::binary);
    {
        Archive ar(os);
        ar << obj;
    }
    os.close();
}

// TODO move to serialization.hpp
template <typename Archive, typename Filter, typename Object>
void save_compr(const char* file, const Object& obj) {
    ofstream os(file, ios::out | ios::binary);
    {
        boost::iostreams::filtering_stream<boost::iostreams::output> fs;
        fs.push(Filter());
        fs.push(os);

        Archive ar(fs);
        ar << obj;
    }
    os.close();
}

// TODO move to serialization.hpp
template <typename Archive, typename Object>
void load(const char* file, Object& obj) {
    ifstream is(file, ios::in | ios::binary);
    {
        Archive ar(is);
        ar >> obj;
    }
    is.close();
}

// TODO move to serialization.hpp
template <typename Archive, typename Filter, typename Object>
void load_compr(const char* file, Object& obj) {
    ifstream is(file, ios::in | ios::binary);
    {
        boost::iostreams::filtering_stream<boost::iostreams::input> fs;
        fs.push(Filter());
        fs.push(is);

        Archive ar(fs);
        ar >> obj;
    }
    is.close();
}

template <typename InputArchive, typename OutputArchive>
void doTest(const Mat& input, const char* filename, bool compression) {
    if (compression)
        save_compr<OutputArchive, boost::iostreams::gzip_compressor, Mat>(filename, input);
    else
        save<OutputArchive, Mat>(filename, input);

    Mat loaded;
    if (compression)
        load_compr<InputArchive, boost::iostreams::gzip_decompressor, Mat>(filename, loaded);
    else
        load<InputArchive, Mat>(filename, loaded);
    BOOST_CHECK(vis::equals(loaded, input));
}

BOOST_AUTO_TEST_CASE(matrix_serialization) {
    Mat mat(4, 4, CV_32F);
    cv::randu(mat, cv::Scalar(0), cv::Scalar(255));

    // plain text
    doTest<boost::archive::text_iarchive, boost::archive::text_oarchive>(mat, "matrix.txt", false);
    doTest<boost::archive::text_iarchive, boost::archive::text_oarchive>(mat, "matrix.txt.gz", true);

    // binary
    doTest<boost::archive::binary_iarchive, boost::archive::binary_oarchive>(mat, "matrix.bin", false);
    doTest<boost::archive::binary_iarchive, boost::archive::binary_oarchive>(mat, "matrix.bin.gz", true);

}

