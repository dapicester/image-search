#define BOOST_TEST_MODULE utils_ported
#include <boost/test/unit_test.hpp>

#include "utils/ported.hpp"
#include "test_utils.hpp"
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

// TODO refactor
#define PRINT 0

BOOST_AUTO_TEST_CASE(test_colon) {
    Mat expected, actual, indices;
    Mat input = (Mat_<int>(3, 4) << 1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 10, 11, 12);

    /* column subset */
    expected = (Mat_<int>(3, 2) << 1, 3,
                                   5, 7,
                                   9, 11);
    indices = (Mat_<int>(1,2) << 0, 2);
    actual = colon(input, indices, COLUMNS);
#if PRINT
    cout << "input" << endl << input << endl;
    cout << "indices" << endl << indices << endl;
    cout << "expected" << endl << expected << endl;
    cout << "actual" << endl << actual << endl;
#endif
    BOOST_CHECK(equals(expected, actual));
    BOOST_CHECK(equals(expected, colon(input, indices.t(), COLUMNS)));

    /* row subset */
    expected = (Mat_<int>(2, 4) << 1, 2, 3, 4,
                                   5, 6, 7, 8);
    indices = (Mat_<int>(1, 2) << 0, 1);
    actual = colon(input, indices.t(), ROWS);
#if PRINT
    cout << "input" << endl << input << endl;
    cout << "indices" << endl << indices << endl;
    cout << "expected" << endl << expected << endl;
    cout << "actual" << endl << actual << endl;
#endif
    BOOST_CHECK(equals(expected, actual));
    BOOST_CHECK(equals(expected, colon(input, indices.t(), ROWS)));
}

