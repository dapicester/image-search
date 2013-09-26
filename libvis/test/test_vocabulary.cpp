/**
 * @file test_vocabulary.cpp
 * @brief Test for visual words vocabulary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE vocabulary
#include <boost/test/unit_test.hpp>

#include "vocabulary.hpp"
#include "utils/filesystem.hpp"
#include <iostream>

using namespace boost::filesystem;
using namespace std;
using namespace vis;

const static path IMAGE_DIR = current_path() / "images";

#define PRINT 1

BOOST_AUTO_TEST_CASE(test_vocabulary) {
    vector<path> files = getImageFiles(IMAGE_DIR);
#if PRINT
    copy(files.begin(), files.end(), ostream_iterator<path>(cout, "\n"));
#endif

    // boost fs and load image paths in ROOT/images
    // compute vocabulary
    // check it's the same as matlab
}

