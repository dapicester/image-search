/**
 * @file test_index.cpp
 * @brief Test for index computation and serialization.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE index
#include <boost/test/unit_test.hpp>

#include "callbacks.hpp"
#include "extract.hpp"
#include "fixtures.hpp"
#include "index.hpp"
#include "vocabulary.hpp"
#include "test_commons.hpp"
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
        BOOST_REQUIRE_EQUAL(descriptors.get().n_cols, files.size());
        BOOST_REQUIRE_EQUAL(descriptors.get().n_rows, cb.length());
    }

    {
        // 2. build index

        vis::Index index;
        index.build("test", descriptors);

        // 3. save
        test::save(INDEX_FILE, index);
    }

    {
        // 4. load

        IndexPtr index(test::load<vis::Index>(INDEX_FILE));
        BOOST_REQUIRE_EQUAL("test", index->getCategory());
        BOOST_REQUIRE_EQUAL(vis::HOG_HSV, index->getType());

        // 5. query

        const size_t queryId = 5;
        arma::fmat query(descriptors.get().col(queryId));

        // single result
        std::vector<vis::Index::id_type> match;
        index->query(query, match);
        BOOST_REQUIRE_EQUAL(1, match.size());
        BOOST_REQUIRE_EQUAL(queryId, match[0]); // exact match

        // multiple results
        std::vector<vis::Index::id_type> results;
        index->query(query, results, 5);
        BOOST_REQUIRE_EQUAL(5, results.size());
        BOOST_REQUIRE_EQUAL(queryId, results[0]); // idem
    }
}

