/**
 * @file test_index.cpp
 * @brief Test for index computation and serialization.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE index
#include <boost/test/unit_test.hpp>

#include "extract.hpp"
#include "fixtures.hpp"
#include "index.hpp"
#include "vocabulary.hpp"
#include "serialization.hpp"

vis::Vocabulary* loadVocabulary() {
    static const boost::filesystem::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(boost::filesystem::is_regular_file(vocabularyFile),
                          "Cannot find vocabulary file: " << vocabularyFile);

    vis::Vocabulary* vp = vis::Vocabulary::load(vocabularyFile);
    BOOST_CHECK(vp);

    return vp;
};

BOOST_FIXTURE_TEST_CASE(test_index, test::ImageDir) {
    cv::Mat descriptors;

    {
        // 1. compute histograms/load from file

        boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );
        vis::CompositeCallback cb(vocabulary.get());
        vis::extract(files, descriptors, cb);
        BOOST_REQUIRE_EQUAL(descriptors.size(),
                            cv::Size(files.size(), cb.getNumBins() + vocabulary->getNumWords()));
    }

    {
        // 2. build index

        vis::Index index;
        index.build("test", descriptors, vis::HOG_HSV);

        // 3. serialize

        vis::GzipSerializer<vis::Index>::Saver save;
        save("index.dat.gz", index);
    }

    {
        // 4. load

        vis::GzipSerializer<vis::Index>::Loader load;
        vis::Index index;
        load("index.dat.gz", index);
        BOOST_REQUIRE_EQUAL("test", index.getCategory());
        BOOST_REQUIRE_EQUAL(vis::HOG_HSV, index.getType());

        // 5. query

        const size_t queryId = 5;
        cv::Mat query(descriptors.col(queryId));

        // single result
        std::vector<vis::Index::id_type> match;
        index.query(query, match);
        BOOST_REQUIRE_EQUAL(1, match.size());
        BOOST_REQUIRE_EQUAL(queryId, match[0]); // exact match

        // multiple results
        std::vector<vis::Index::id_type> results;
        index.query(query, results, 5);
        BOOST_REQUIRE_EQUAL(5, results.size());
        BOOST_REQUIRE_EQUAL(queryId, results[0]); // idem
    }
}

