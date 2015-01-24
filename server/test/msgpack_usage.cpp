/**
 * @file msgpack_usage.cpp
 * @brief Usage test for msgpack library
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE msgpack
#include <boost/test/unit_test.hpp>

#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

BOOST_AUTO_TEST_CASE(msgpack_usage) {
    // This is source message.
    std::vector<std::string> source = { "Hello,", "World!" };

    // Serialize it.
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, source);

    // Transmit it.
    std::printf("buffer [%zu] => \"%s\"\n", sbuf.size(), sbuf.data());

    // Deserialize the serialized data.
    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    msgpack::object obj = msg.get();

    // Print the deserialized object.
    std::ostringstream os;
    os << obj << std::endl;
    BOOST_CHECK_EQUAL(os.str(), "[\"Hello,\", \"World!\"]\n");

    // Convert the deserialized object to statically typed object.
    std::vector<std::string> destination;
    obj.convert(&destination);
    BOOST_CHECK_EQUAL_COLLECTIONS(destination.begin(), destination.end(),
                                  source.begin(), source.end());

    // If the type is mismatched, it throws msgpack::type_error.
    BOOST_CHECK_THROW(obj.as<int>(), msgpack::type_error);
}
