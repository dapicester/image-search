/**
 * @file recompute.hpp
 * @brief Utility for recomputing descriptors and indexes.
 * @author Paolo D'Apice
 */

#ifndef DEMO_RECOMPUTE_HPP
#define DEMO_RECOMPUTE_HPP

#include <QString>
#include <QScopedPointer>
#include <boost/filesystem/path.hpp>

namespace vis {
    class Descriptors;
    class Index;
    class Vocabulary;
}

namespace fs = boost::filesystem;

class Recompute {
    typedef std::vector<fs::path> vector_path;

public:
    Recompute(const char* dataPath);
    Recompute(const fs::path& dataPath);
    ~Recompute();

    void recompute(const QString& category, const QString& type);

private:
    void loadImagePaths(const QString& category);
    void loadQueryImagePaths(const QString& category);
    void computeVocabulary(const QString& category);
    void computeDescriptors(const QString& category, const QString& type);
    void computeQueries(const QString& category, const QString& type);
    void computeIndex(const QString& category, const QString& type);

private:
    const fs::path dataPath;

    vector_path images;
    vector_path queryImages;
    QScopedPointer<vis::Vocabulary> vocabulary;
    QScopedPointer<vis::Descriptors> descriptors;
    QScopedPointer<vis::Descriptors> queries;
    QScopedPointer<vis::Index> index;
};

#endif

