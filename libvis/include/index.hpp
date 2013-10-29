/**
 * @file index.hpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#ifndef VIS_INDEX_HPP
#define VIS_INDEX_HPP

#include "descriptors_type.hpp"
#include "descriptors.hpp"
#include <opencv2/core/core.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/scoped_ptr.hpp>

namespace vis {

template <typename T> class KDTree;

/// Index structure.
class Index {
public:
    /// Default constructor.
    Index();

    ~Index();

    /**
     * @brief Build an index.
     * @param category A string identifier for this index.
     * @param data Matrix containing N rows where the \a n-th row corresponds to \a n-th indexed image.
     * @param type Descriptors type.
     * @param numTree Number of trees in the KD-forest.
     */
    void build(const std::string& category,
               const cv::Mat& data,
               DescriptorsType type,
               size_t numTrees = 1);

    /**
     * @brief Build an index.
     * @param category A string identifier for this index.
     * @param type Descriptors wrapper.
     * @param numTree Number of trees in the KD-forest.
     */
    void build(const std::string& category,
               const Descriptors& descriptors,
               size_t numTrees = 1);

    /// @brief Type of the returned id used for querying.
    typedef size_t id_type;

    /**
     * @brief Query the index.
     * @param[in] data Query data.
     * @param[out] results Container for results.
     * @param[in] numResults Number of query results requested.
     * @param[in] maxNumComparisons Maximum number of comparisons (ANN), \c 0 means unbounded.
     */
    void query(const cv::Mat& data,
               std::vector<id_type>& results,
               size_t numResults = 1,
               size_t maxNumComparisons = 0) const;

    std::string getCategory() const;

    DescriptorsType getType() const;

    /// @brief Read index from file.
    static Index* load(const boost::filesystem::path& file);

    /// @brief Write index to file.
    void save(const boost::filesystem::path& file);

private:
    std::string category;
    DescriptorsType type;
    boost::scoped_ptr< KDTree<float> > kdtree;

private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version);
};

inline std::string
Index::getCategory() const {
    return category;
}

inline DescriptorsType
Index::getType() const {
    return type;
}

template <typename Archive>
void
Index::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & type;
    ar & kdtree;
}

} /* namespace vis */

#endif /* VIS_INDEX_HPP */

