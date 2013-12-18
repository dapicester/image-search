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

class Descriptors;
class Index;
class Vocabulary;

/// Façade class to the image search system.
class ImageSearch {
public:

    /**
     * Instantiate a new search system.
     * @param category Image category.
     * @param type Descriptors used.
     * @param dataDir Path the data directory containing images.
     */
    ImageSearch(const std::string& category, DescriptorsType type,
            const boost::filesystem::path& dataDir);

    /// Destructor.
    ~ImageSearch();

    /// Load data from saved files.
    void load();

    /// Process and index images.
    void build();

    /// Save data to files.
    void save() const;

    /**
     * Query by image id.
     * @param id Query image id.
     * @param[out] results Search results.
     */
    void query(unsigned id, std::vector<size_t>& results) const;

    /**
     * Query by image descriptors.
     * @param descriptors Image descriptors.
     * @param[out] results Search results.
     */
    void query(const arma::fvec& descriptors, std::vector<size_t>& results) const;

    /**
     * Query by image.
     * @param image Image as read with OpenCV.
     * @param[out] results Search results.
     */
    void query(const cv::Mat& image, std::vector<size_t>& results) const;

    /// Extract descriptors from image.
    arma::fvec extract(const cv::Mat& image) const;

private:

    void loadIndex();
    void loadVocabulary();

    void buildIndex();
    void buildDescriptors();
    void buildVocabulary();

    void saveIndex() const;
    void saveDescriptors() const;
    void saveVocabulary() const;

private:
    std::string category;
    DescriptorsType type;

    boost::filesystem::path dataDir;

    boost::scoped_ptr<Index> index;
    boost::scoped_ptr<Descriptors> descriptors;
    boost::scoped_ptr<Vocabulary> vocabulary;
};

}

#endif // VIS_IMSEARCH_HPP

