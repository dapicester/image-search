/**
 * @file test_commons.hpp
 * @brief Common functions used for testing.
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_COMMONS_HPP
#define VIS_TEST_COMMONS_HPP

#include <boost/filesystem.hpp>
#include <functional>

namespace fs = boost::filesystem;

namespace test {

auto progress = [](int i, const std::vector<fs::path>& files) {
    std::cout << "  Extracting features from " << files[i]
              << " (" << i+1 << "/" << files.size() << ")" << std::endl;
};

template <typename Object>
Object*
load(const fs::path& file) {
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(file), "Cannot find file: " << file);

    std::cout << "Loading from file: " << file << " ... ";
    Object* ptr = Object::load(file);
    BOOST_REQUIRE(ptr);
    std::cout << "done" << std::endl;

    return ptr;
};

template <typename Object>
void save(const fs::path file, const Object& obj) {
    if (fs::exists(file))
        fs::remove(file);
    BOOST_CHECK(not fs::exists(file));

    std::cout << "Saving to file: " << file << " ... ";
    obj.save(file);
    BOOST_CHECK(fs::is_regular_file(file));
    std::cout << "done" << std::endl;
}

} /* namespace test */

#endif /* VIS_TEST_COMMONS_HPP */

