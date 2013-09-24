/**
 * @file test_vocabulary.cpp
 * @brief Test for visual words vocabulary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE vocabulary
#include <boost/test/unit_test.hpp>

#include "vocabulary.hpp"
#include "utils/filesystem.hpp"
#include "utils/print.hpp"
#include <boost/scoped_ptr.hpp>

using namespace boost::filesystem;
using boost::scoped_ptr;
using namespace std;
using namespace vis;

//const static path IMAGE_DIR = current_path() / "images";
const static path IMAGE_DIR = current_path() / "data" / "test";

BOOST_AUTO_TEST_CASE(test_vocabulary) {
    vector<path> files = getImageFiles(IMAGE_DIR);

    // arrange as matlab
    std::reverse(files.begin(), files.end());
    std::sort(files.begin(), files.end());

    // compute vocabulary
    scoped_ptr<Vocabulary> vocabulary(Vocabulary::fromImageList("test", files));
    BOOST_CHECK(vocabulary.get());

    // check it's the same as matlab
}

