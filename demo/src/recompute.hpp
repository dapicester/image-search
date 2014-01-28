/**
 * @file recompute.hpp
 * @brief Utility for recomputing descriptors and indexes.
 * @author Paolo D'Apice
 */

#ifndef DEMO_RECOMPUTE_HPP
#define DEMO_RECOMPUTE_HPP

#include <boost/filesystem/path.hpp>
#include <memory>

namespace vis {
    class Descriptors;
    class Index;
    class Vocabulary;
}

namespace fs = boost::filesystem;

class Recompute {
    typedef std::vector<fs::path> vector_path;

public:
    Recompute(const fs::path& dataPath);
    ~Recompute();

    void recompute(const std::string& category, const std::string& type);

private:
    void loadImagePaths(const std::string& category);
    void loadQueryImagePaths(const std::string& category);
    void computeVocabulary(const std::string& category);
    void computeDescriptors(const std::string& category, const std::string& type);
    void computeQueries(const std::string& category, const std::string& type);
    void computeIndex(const std::string& category, const std::string& type);

private:
    const fs::path dataPath;

    vector_path images;
    vector_path queryImages;
    std::unique_ptr<vis::Vocabulary> vocabulary;
    std::unique_ptr<vis::Descriptors> descriptors;
    std::unique_ptr<vis::Descriptors> queries;
    std::unique_ptr<vis::Index> index;
};

#endif

