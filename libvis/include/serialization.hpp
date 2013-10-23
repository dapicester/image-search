/**
 * @file serialization.hpp
 * @brief Data serialization
 * @author Paolo D'Apice
 */

#ifndef VIS_SERIALIZATION_HPP
#define VIS_SERIALIZATION_HPP

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem/path.hpp>
#include <fstream>

namespace vis {

namespace ar = boost::archive;
namespace io = boost::iostreams;
namespace fs = boost::filesystem;

/// @brief Save object to binary file.
template <typename OutputArchive, typename Object>
struct serializer {
    void operator()(const char* file, const Object& obj) const {
        std::ofstream os(file, std::ios::out | std::ios::binary);
        {
            OutputArchive ar(os);
            ar << obj;
        }
        os.close();
    }
    void operator()(const fs::path& path, const Object& obj) const {
        operator()(path.string().c_str(), obj);
    }
};

/// @brief Save object to compressed binary file.
template <typename OutputArchive, typename Object, typename Compressor>
struct compress_serializer {
    void operator()(const char* file, const Object& obj) const {
        std::ofstream os(file, std::ios::out | std::ios::binary);
        {
            io::filtering_stream<io::output> fs;
            fs.push(Compressor());
            fs.push(os);

            OutputArchive ar(fs);
            ar << obj;
        }
        os.close();
    }
    void operator()(const fs::path& path, const Object& obj) const {
        operator()(path.string().c_str(), obj);
    }
};

/// @brief Load object from binary file.
template <typename InputArchive, typename Object>
struct deserializer {
    void operator()(const char* file, Object& obj) const {
        std::ifstream is(file, std::ios::in | std::ios::binary);
        {
            InputArchive ar(is);
            ar >> obj;
        }
        is.close();
    }
    void operator()(const fs::path& path, Object& obj) const {
        operator()(path.string().c_str(), obj);
    }
};

/// @brief Load object from compressed binary file.
template <typename InputArchive, typename Object, typename Decompressor>
struct compress_deserializer {
    void operator()(const char* file, Object& obj) const {
        std::ifstream is(file, std::ios::in | std::ios::binary);
        {
            io::filtering_stream<io::input> fs;
            fs.push(Decompressor());
            fs.push(is);

            InputArchive ar(fs);
            ar >> obj;
        }
        is.close();
    }
    void operator()(const fs::path& path, Object& obj) const {
        operator()(path.string().c_str(), obj);
    }
};

/// @brief Binary serialization/deserialization objects container.
template <typename Object>
struct BinarySerializer {
    typedef serializer<ar::binary_oarchive, Object> Saver;
    typedef deserializer<ar::binary_iarchive, Object> Loader;
};

/// @brief Compressed binary serialization/deserialization objects container.
template <typename Object>
struct GzipSerializer {
    typedef compress_serializer<ar::binary_oarchive, Object, io::gzip_compressor> Saver;
    typedef compress_deserializer<ar::binary_iarchive, Object, io::gzip_decompressor> Loader;
};

/// @brief Load object from file.
template <typename Object, template <typename> class Serializer>
Object*
load(const fs::path& file) {
    typename Serializer<Object>::Loader loader;
    Object* obj = new Object;
    loader(file.string().c_str(), *obj);
    return obj;
}

/// @brief Save object to file.
template <typename Object, template <typename> class Serializer>
void
save(const fs::path& file, const Object& obj) {
    typename Serializer<Object>::Saver saver;
    saver(file.string().c_str(), obj);
}

} /* namespace vis */

#include "detail/serialization_matrix.hpp"
#include "detail/serialization_kdtree.hpp"

#endif /* VIS_SERIALIZATION_HPP */

