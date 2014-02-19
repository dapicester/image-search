/**
 * @file test_descriptors.cpp
 * @brief Test for descriptors computation.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE descriptors
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include "test_commons.hpp"

#include "vis/callbacks.hpp"
#include "vis/descriptors.hpp"
#include "vis/vocabulary.hpp"
#include "vis/utils/handlers.hpp"

#include <boost/scoped_ptr.hpp>

BOOST_FIXTURE_TEST_SUITE(descriptors, test::ImageDir)

static const fs::path VOCABULARY_FILE = "test_vocabulary.dat";

typedef boost::scoped_ptr<vis::Callback>    CallbackPtr;
typedef boost::scoped_ptr<vis::Vocabulary>  VocabularyPtr;
typedef boost::scoped_ptr<vis::Descriptors> DescriptorsPtr;

void checkDescriptors(const vis::Callback& callback,
        const vis::Descriptors& descriptors,
        const std::vector<fs::path>& files) {
    const arma::fmat& data = descriptors.data();
    const std::vector<fs::path>& indexedFiles = descriptors.getFiles();

    BOOST_CHECK_EQUAL(callback.length(),  data.n_rows);
    BOOST_CHECK_EQUAL(files.size(), data.n_cols);
    BOOST_CHECK_EQUAL(callback.type(), descriptors.getType());
    BOOST_CHECK_EQUAL_COLLECTIONS(files.begin(), files.end(),
                                  indexedFiles.begin(), indexedFiles.end());
}

void testSerialization(const fs::path& file,
        const vis::Descriptors& descriptors,
        const std::vector<fs::path>& files) {
    test::save(file, descriptors);

    DescriptorsPtr loaded(test::load<vis::Descriptors>(file));
    const std::vector<fs::path>& indexedFiles = loaded->getFiles();

    BOOST_CHECK_EQUAL(descriptors.getCategory(), loaded->getCategory());
    BOOST_CHECK(test::equals(descriptors.data(), loaded->data()));
    BOOST_CHECK_EQUAL(descriptors.getType(), loaded->getType());
    BOOST_CHECK_EQUAL_COLLECTIONS(files.begin(), files.end(),
                                  indexedFiles.begin(), indexedFiles.end());
}

#define PROGRESS [&](int i) { vis::handlers::PrintFile(i, files); }

BOOST_AUTO_TEST_CASE(hog) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
    CallbackPtr callback(vis::getCallback(vis::HOG, vocabulary.get()));
    BOOST_CHECK_EQUAL(vis::HOG, callback->type());

    vis::Descriptors descriptors;
    descriptors.compute("test", files, *callback, vis::ColorMode::GRAYSCALE, PROGRESS);

    checkDescriptors(*callback, descriptors, files);

    testSerialization("test_descriptors_hog.dat", descriptors, files);
}

BOOST_AUTO_TEST_CASE(hsv) {
    CallbackPtr callback(vis::getCallback(vis::HSV));
    BOOST_CHECK_EQUAL(vis::HSV, callback->type());

    vis::Descriptors histograms;
    histograms.compute("test", files, *callback, vis::ColorMode::COLORS, PROGRESS);

    checkDescriptors(*callback, histograms, files);

    testSerialization("test_descriptors_hsv.dat", histograms, files);
}

BOOST_AUTO_TEST_CASE(hoghsv) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));

    /*
     * TODO this is the interface I want
     *
     * HogBagOfWordsCallback hog(vocabulary.data());
     * HsvHistogramsCallback hsv;
     *
     * Extractor extractor;
     * extractor.set(hog);
     * extractor.set(hsv);
     *
     * cv::Mat descriptors;
     * extractor.extract(files, descriptors);
     *
     * size_t descriptorSize = hsv.length() + hog.length();
     */

    CallbackPtr callback(vis::getCallback(vis::HOG_HSV, vocabulary.get()));
    BOOST_CHECK_EQUAL(vis::HOG_HSV, callback->type());

    vis::Descriptors descriptors;
    descriptors.compute("test", files, *callback, vis::ColorMode::COLORS, PROGRESS);

    checkDescriptors(*callback, descriptors, files);

    testSerialization("test_descriptors_hoghsv.dat", descriptors, files);
}

BOOST_AUTO_TEST_SUITE_END()

