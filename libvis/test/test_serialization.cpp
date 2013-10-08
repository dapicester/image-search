/**
 * @file test_serialization.cpp
 * @brief Test for data serialization
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE serialization
#include <boost/test/unit_test.hpp>

#include "kdtree.hpp"
#include "utils/data.hpp"
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
using std::vector;

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

BOOST_AUTO_TEST_CASE(vlkdforest_serialization) {
    int dimensions = 2;
    int numData = 100;
    float* data = getTestDataPtr<float>(dimensions, numData);

    BinarySerializer<VlKDForest*>::Saver saver;
    BinarySerializer<VlKDForest*>::Loader loader;

    {
        VlKDForest* forest = vl_kdforest_new(VL_TYPE_FLOAT, dimensions, 1, VlDistanceL2);
        vl_kdforest_set_thresholding_method (forest, VL_KDTREE_MEDIAN);
        vl_kdforest_build(forest, numData, data);

        saver("vlkdforest.dat", forest);

        vl_kdforest_delete(forest);
    }
    {
        // FIXME memory leak in this block

        VlKDForest* forest;
        loader("vlkdforest.dat", forest);

        BOOST_CHECK(not forest->data);
        forest->data = data;

        BOOST_CHECK_EQUAL(VL_TYPE_FLOAT, vl_kdforest_get_data_type(forest));
        BOOST_CHECK_EQUAL(VL_KDTREE_MEDIAN, vl_kdforest_get_thresholding_method(forest));

        {
            // query
            srand(time(NULL));
            vl_index index = rand() % numData;
            float* query = data + index * dimensions;

            VlKDForestNeighbor* neighbors = (VlKDForestNeighbor*) vl_malloc(sizeof(VlKDForestNeighbor));

            vl_kdforest_set_max_num_comparisons(forest, 0);
            vl_kdforest_query(forest, neighbors, 1, query);

            BOOST_CHECK_EQUAL(index, neighbors->index);
            BOOST_CHECK_EQUAL(0.0, neighbors->distance);

            vl_free(neighbors);
        }

        BOOST_CHECK(forest);
        vl_kdforest_delete(forest);
    }

    delete[] data;
}

BOOST_AUTO_TEST_CASE(kdtree_serialization) {
    typedef KDTree<float> Tree;

    int dimension = 2;
    int numData = 100;
    Mat data = getTestData<float>(dimension, numData);

    {
        BinarySerializer<Tree>::Saver saver;

        Tree tree(data);
        saver("kdtree.dat", tree);
    }
    {
        BinarySerializer<Tree>::Loader loader;

        Tree tree;
        loader("kdtree.dat", tree);

        // query
        srand(time(NULL));
        int index = rand() % numData;
        Mat query = data.col(index).clone();

        vector<KDTreeNeighbor> results = tree.search(query);
        BOOST_CHECK_EQUAL(1, results.size());

        KDTreeNeighbor record = results.front();
        BOOST_CHECK_EQUAL(index, record.index);
        BOOST_CHECK_EQUAL(0.0, record.distance);
    }
}

