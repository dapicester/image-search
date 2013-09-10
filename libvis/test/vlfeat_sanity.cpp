/**
 * @file vlfeat_sanity.cpp
 * @brief Sanity test for the VLFeat libraries
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE sanity
#include <boost/test/unit_test.hpp>

extern "C" {
#include <vl/generic.h>
}

BOOST_AUTO_TEST_CASE(sanity) {
    VL_PRINT(vl_get_version_string());
    VL_PRINT(vl_configuration_to_string_copy());
}

