/**
 * @file test_commons.hpp
 * @brief Common functions used for testing.
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_COMMONS_HPP
#define VIS_TEST_COMMONS_HPP

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace test {

template <typename Object>
Object*
load(const fs::path& file) {
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(file), "Cannot find file: " << file);

    Object* ptr = Object::load(file);
    BOOST_REQUIRE(ptr);

    return ptr;
};

template <typename Object>
void save(const fs::path file, const Object& obj) {
    if (fs::exists(file))
        fs::remove(file);
    BOOST_CHECK(not fs::exists(file));

    obj.save(file);
    BOOST_CHECK(fs::is_regular_file(file));
}

} /* namespace test */

#endif /* VIS_TEST_COMMONS_HPP */

