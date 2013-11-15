/**
 * @file vocabulary.hpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#ifndef VIS_VOCABULARY_HPP
#define VIS_VOCABULARY_HPP

#include "extract.hpp"
#include "kdtree.hpp"
#include "utils/subset.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <string>
#include <vector>

namespace vis {

namespace vocabulary {
    static const size_t NUM_WORDS = 300; ///< Default number of dictionary words.
}

class Vocabulary;

/// @brief Callback for computing vocabulary
template <typename Extractor>
struct VocabularyCallback {
    arma::fmat operator()(const cv::Mat& image) const {
        return subset(e.extract(image), n);
    }

private:
    VocabularyCallback(size_t nn) : n(nn) {}

    const size_t n;
    const Extractor e;

    friend class Vocabulary;
};

/// @brief Vocabulary of visual words of HOG descriptors.
class Vocabulary : private boost::noncopyable {
public:
    /// Default constructor.
    Vocabulary(size_t numWords = vocabulary::NUM_WORDS);

    /// Default destructor.
    ~Vocabulary();

    /// @brief Build vocabulary on data.
    void build(const std::string category, const arma::fmat& data);

    /// @brief Computes vocabulary for the given image file paths.
    template <class Extractor>
    static Vocabulary* fromImageList(
        const std::string& category,
        const std::vector<boost::filesystem::path>& names,
        size_t numWords = vocabulary::NUM_WORDS,
        ProgressHandler handler = [](int){});

    /// @brief Read vocabulary from file.
    static Vocabulary* load(const boost::filesystem::path& file);

    /// @brief Write vocabulary to file.
    void save(const boost::filesystem::path& file) const;

    /// @brief Quantize the input descriptors into words.
    arma::fmat quantize(const arma::fmat& descriptors) const;

    /// @brief Lookup the input descriptors as words (indices).
    arma::uvec lookup(const arma::fmat& descriptors) const;

    /// @return The actual number of words in dictionary.
    size_t getNumWords() const { return numWords; }

    /// @return The category for which the dictionary has been built.
    std::string getCategory() const { return category; }

private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version);

private:
    size_t numWords;
    std::string category;
    arma::fmat words;
    boost::scoped_ptr<KDTree<float> > kdtree;

#ifdef ENABLE_TESTING
public:
    const KDTree<float>* getKDTree() const { return kdtree.get(); }
    const arma::fmat& getWords() const { return words; }
#endif
};

template <typename Archive>
void
Vocabulary::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & numWords;
    ar & words;
    ar & kdtree;
}

template <class Extractor>
inline Vocabulary*
Vocabulary::fromImageList(
        const std::string& category,
        const std::vector<boost::filesystem::path>& names,
        size_t numWords,
        ProgressHandler handler) {
    const size_t len = names.size();
    const size_t numFeatures = cvRound(numWords*100.0/len);

    arma::fmat descriptors;

    VocabularyCallback<Extractor> cb(numFeatures);
    extract(names, descriptors, cb, ColorMode::COLORS, handler);

    Vocabulary* vocabulary = new Vocabulary(numWords);
    vocabulary->build(category, descriptors);

    return vocabulary;
}

} /* namespace vis */

#endif /* VIS_VOCABULARY_HPP */

