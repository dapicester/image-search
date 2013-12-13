/**
 * @file imsearch.hpp
 * @brief Façade class for the image search system.
 * @author Paolo D'Apice
 */

#ifndef VIS_IMSEARCH_HPP
#define VIS_IMSEARCH_HPP

#include "vis/descriptors_type.hpp"

#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

namespace vis {

class Index;
class Vocabulary;

class ImageSearch {
public:

    ImageSearch(const std::string& category, DescriptorsType type,
            const boost::filesystem::path& dataDir);
    ~ImageSearch();

    void load();
    void build();

    void query(unsigned id, std::vector<size_t>& results) const;
    void query(const arma::fvec& descriptors, std::vector<size_t>& results) const;
    void query(const cv::Mat& image, std::vector<size_t>& results) const;

    arma::fvec extract(const cv::Mat& image) const;
private:

    void loadIndex();
    void loadVocabulary();

    void buildIndex();
    void buildVocabulary();

    void saveIndex() const;
    void saveVocabulary() const;

private:
    std::string category;
    DescriptorsType type;

    boost::filesystem::path dataDir;

    boost::scoped_ptr<Index> index;
    boost::scoped_ptr<Vocabulary> vocabulary;
};

}

#endif // VIS_IMSEARCH_HPP

