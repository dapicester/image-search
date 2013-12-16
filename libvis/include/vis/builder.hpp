/**
 * @file builder.hpp
 * @brief Build descriptors and indexes.
 * @author Paolo D'Apice
 */

#ifndef VIS_BUILDER_HPP
#define VIS_BUILDER_HPP

#include "vis/descriptors_type.hpp"

#include <boost/filesystem/path.hpp>

namespace vis {

class Descriptors;
class Index;
class Vocabulary;

class Builder {
public:
    Builder(const boost::filesystem::path& dataPath,
            const std::string& category,
            DescriptorsType type);
    ~Builder();

    // TODO expose handler argument

    Vocabulary* computeVocabulary();
    Descriptors* computeDescriptors(const Vocabulary* vocabulary = nullptr);
    Index* computeIndex(const Descriptors* descriptors);

private:
    void loadImageNames();

private:
    const boost::filesystem::path& dataPath;
    const std::string& category;
    const DescriptorsType& type;

    typedef std::vector<boost::filesystem::path> vector_path;
    vector_path images;
};

} // namespace vis

#endif // VIS_BUILDER_HPP

