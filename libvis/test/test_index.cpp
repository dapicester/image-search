/**
 * @file test_index.cpp
 * @brief Test for index computation and serialization.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE index
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "test_commons.hpp"

#include "vis/callbacks.hpp"
#include "vis/extract.hpp"
#include "vis/index.hpp"
#include "vis/vocabulary.hpp"

#include <boost/scoped_ptr.hpp>

static const fs::path VOCABULARY_FILE = "test_vocabulary.dat";
static const fs::path INDEX_FILE = "index.dat.gz";

typedef boost::scoped_ptr<vis::Vocabulary> VocabularyPtr;
typedef boost::scoped_ptr<vis::Descriptors> DescriptorsPtr;
typedef boost::scoped_ptr<vis::Index> IndexPtr;

BOOST_FIXTURE_TEST_CASE(test_index, test::ImageDir) {
    vis::Descriptors descriptors;

    {
        // 1. compute histograms/load from file

        VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
        vis::CompositeCallback cb(*vocabulary);

        descriptors.compute("test", files, cb);
        BOOST_REQUIRE_EQUAL(descriptors.data().n_cols, files.size());
        BOOST_REQUIRE_EQUAL(descriptors.data().n_rows, cb.length());
    }

    {
        // 2. build index

        vis::Index index;
        index.build("test", descriptors);

        auto indexed = index.getFiles();
        BOOST_REQUIRE_EQUAL_COLLECTIONS(indexed.begin(), indexed.end(),
                                        files.begin(), files.end());

        // 3. save
        test::save(INDEX_FILE, index);
    }

    {
        // 4. load

        IndexPtr index(test::load<vis::Index>(INDEX_FILE));
        BOOST_REQUIRE_EQUAL("test", index->getCategory());
        BOOST_REQUIRE_EQUAL(vis::HOG_HSV, index->getType());

        auto indexed = index->getFiles();
        BOOST_REQUIRE_EQUAL_COLLECTIONS(indexed.begin(), indexed.end(),
                                        files.begin(), files.end());

        // 5. query

        const arma::uvec queryId{5};
        arma::fmat query(descriptors.data().cols(queryId));

        // by value, single result
        std::vector<vis::Index::id_type> match;
        index->query(query, match);
        BOOST_REQUIRE_EQUAL(1, match.size());
        BOOST_REQUIRE_EQUAL(queryId[0], match[0]);  // exact match

        // by index, multiple results
        unsigned numResults = 10;
        std::vector<vis::Index::id_type> results;
        index->query(queryId, results, numResults);
        BOOST_REQUIRE_EQUAL(numResults, results.size());
        BOOST_REQUIRE_EQUAL(queryId[0], match[0]);  // exact match
        std::for_each(match.begin() + 1, match.end(), [&](const vis::Index::id_type& id) {
            BOOST_CHECK_NE(queryId[0], id);         // non-exact match
        });
    }
}

