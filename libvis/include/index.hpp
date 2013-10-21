/**
 * @file index.hpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#ifndef VIS_INDEX_HPP
#define VIS_INDEX_HPP

#include <opencv2/core/core.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/access.hpp>

namespace vis {

template <typename T> class KDTree;

/// Available index types.
enum IndexType {
    HOG,        ///< Index using HOG descriptors.
    HSV,        ///< Index using HSV color histograms.
    HOG_HSV,    ///< Index using both HOG descriptors and HSV color histograms.
};

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
     * @param type Index type.
     */
    void build(const std::string& category,
               const cv::Mat& data,
               IndexType type);

    /// @brief Type of the returned id used for querying.
    typedef size_t id_type;

    /**
     * @brief Query the index.
     * @param[in] data Query data.
     * @param[out] results Container for results.
     * @param[in] numResults Number of query results requested.
     */
    void query(const cv::Mat& data, std::vector<id_type>& results, size_t numResults = 1);

    std::string getCategory() const;

    IndexType getType() const;

private:
    std::string category;
    IndexType type;
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

inline IndexType
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

