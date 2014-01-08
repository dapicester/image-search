/**
 * @file test_serialization.cpp
 * @brief Test for data serialization
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE serialization
#include <boost/test/unit_test.hpp>

#include "utils/data.hpp"
#include "utils/matrix.hpp"

#include "vis/kdtree.hpp"
#include "vis/serialization.hpp"

#include <opencv2/core/core.hpp>
#include <fstream>
#include <random>

namespace ar = boost::archive;
namespace io = boost::iostreams;
namespace fs = boost::filesystem;

template <typename InputArchive, typename OutputArchive, typename Matrix>
void doTest(const Matrix& input, const char* filename, bool compression) {
    if (compression) {
        static vis::compress_serializer<OutputArchive, Matrix, io::gzip_compressor> GzipSaver;
        GzipSaver(filename, input);
    }
    else {
        static vis::serializer<OutputArchive, Matrix> Saver;
        Saver(filename, input);
    }

    Matrix loaded;
    if (compression) {
        static vis::compress_deserializer<InputArchive, Matrix, io::gzip_decompressor> GzipLoader;
        GzipLoader(filename, loaded);
    }
    else {
        static vis::deserializer<InputArchive, Matrix> Loader;
        Loader(filename, loaded);
    }

    BOOST_CHECK(test::equals(loaded, input));
}

BOOST_AUTO_TEST_CASE(cv_mat_serialization) {
    cv::Mat mat(4, 4, CV_32F);
    cv::randu(mat, cv::Scalar(0), cv::Scalar(255));

    // plain text
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "cv_matrix.txt", false);
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "cv_matrix.txt.gz", true);

    // binary
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "cv_matrix.bin", false);
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "cv_matrix.bin.gz", true);
}

BOOST_AUTO_TEST_CASE(arma_mat_serialization) {
    arma::mat mat(4, 4);
    mat.randu();

    // plain text
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "arma_matrix.txt", false);
    doTest<ar::text_iarchive, ar::text_oarchive>(mat, "arma_matrix.txt.gz", true);

    // binary
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "arma_matrix.bin", false);
    doTest<ar::binary_iarchive, ar::binary_oarchive>(mat, "arma_matrix.bin.gz", true);
}

BOOST_AUTO_TEST_CASE(vlkdforest_serialization) {
    int dimensions = 2;
    int numData = 100;
    float* data = test::getTestDataPtr<float>(dimensions, numData);

    vis::BinarySerializer<VlKDForest*>::Saver saver;
    vis::BinarySerializer<VlKDForest*>::Loader loader;

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
    typedef vis::KDTree<float> Tree;

    int dimension = 2;
    int numData = 100;
    arma::fmat data = test::testData<float>(dimension, numData);

    int numTrees = 2;

    {
        Tree tree;
        tree.build(data, numTrees);

        vis::BinarySerializer<Tree>::Saver saver;
        saver("kdtree.dat", tree);
    }
    {
        vis::BinarySerializer<Tree>::Loader loader;

        Tree tree;
        loader("kdtree.dat", tree);

        BOOST_CHECK_EQUAL(numTrees, tree.getNumTrees());

        // query
        int index = rand() % numData;
        arma::fmat query = data.col(index);

        std::vector<vis::KDTreeNeighbor> results = tree.search<vis::KDTreeNeighbor>(query);
        BOOST_CHECK_EQUAL(1, results.size());

        vis::KDTreeNeighbor record = results.front();
        BOOST_CHECK_EQUAL(index, record.index);
        BOOST_CHECK_EQUAL(0.0, record.distance);
    }
}

BOOST_AUTO_TEST_CASE(path_serialization) {
    {
        fs::path path = "/path/to/file";
        vis::BinarySerializer<fs::path>::Saver saver;
        saver("path.dat", path);
    }
    {
        vis::BinarySerializer<fs::path>::Loader loader;

        fs::path path;
        loader("path.dat", path);

        BOOST_CHECK_EQUAL(path, fs::path("/path/to/file"));
    }
}

