/**
 * @file vocabulary.hpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#ifndef VIS_VOCABULARY_HPP
#define VIS_VOCABULARY_HPP

#include "kdtree.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace vis {

namespace vocabulary {
    static const size_t NUM_WORDS = 300;
    static const vl_size MAX_COMPARISONS = 15;
}

/// @brief Vocabulary of visual words of HOG descriptors.
class Vocabulary : private boost::noncopyable {
public:

    ~Vocabulary();

    /// @brief Computes vocabulary for the given image file paths.
    static Vocabulary* fromImageList(
        const std::string& category,
        const std::vector<boost::filesystem::path>& names,
        size_t numWords = vocabulary::NUM_WORDS);

    /// @brief Read vocabulary from file.
    static Vocabulary* load(const boost::filesystem::path& file);

    /// @brief Write vocabulary to file.
    void save(const boost::filesystem::path& file) const;

    /// @brief Quantize the input descriptors into words.
    cv::Mat quantize(const cv::Mat& descriptors) const;

    size_t getNumWords() const;

private:
    Vocabulary();
    Vocabulary(const std::string category, const cv::Mat& data, size_t numWords);

    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version);

private:
    std::string category;
    size_t numWords;
    cv::Mat words;
    boost::scoped_ptr<KDTree<float> > kdtree;

#ifdef ENABLE_TESTING
public:
    const KDTree<float>* getKDTree() const { return kdtree.get(); }
    const cv::Mat& getWords() const { return words; }
#endif
};

inline size_t
Vocabulary::getNumWords() const {
    return numWords;
}

template <typename Archive>
void
Vocabulary::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & numWords;
    ar & words;
    ar & kdtree;
}

} /* namespace vis */

#endif /* VIS_VOCABULARY_HPP */

