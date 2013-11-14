#define BOOST_TEST_MODULE utils_ported
#include <boost/test/unit_test.hpp>

#include "utils/subset.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"

BOOST_AUTO_TEST_CASE(test_subset_mat) {
    arma::mat input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    input.reshape(1,10);

    //arma::mat expected = { 1, 3, 6, 8, 10 }; // XXX vl_colsubset gives this
    arma::mat expected = { 1, 3, 5, 7, 10 };

    arma::mat actual = vis::subset(input, 5);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_subset_vector) {
    std::vector<std::string> v;
    for (int i=1; i<=100; i++)
        v.push_back(std::to_string(i));
    printvar(v);

    std::vector<std::string> actual = vis::subset(v, 10);
    printvar(actual);

    std::vector<std::string> expected = {"1","12","23","34","45","56","67","78","89","100"};
    printvar(expected);

    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
}


