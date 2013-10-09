/**
 * @file vocabulary.hpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#ifndef VIL_VOCABULARY_H
#define VIL_VOCABULARY_H

#include "kdtree.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
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

    size_t getNumWords() const { return numWords; }

    ~Vocabulary();

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

} /* namespace vis */

#include "detail/serialization_vocabulary.hpp"

#endif /* VIL_VOCABULARY_H */

