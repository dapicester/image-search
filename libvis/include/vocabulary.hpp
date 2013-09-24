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
#include <string>
#include <vector>

namespace vis {

namespace vocabulary {
    static const size_t NUM_WORDS = 300;
}

class Vocabulary : private boost::noncopyable {
public:
    static Vocabulary* fromImageList(
        const std::string& category,
        const std::vector<std::string>& names,
        size_t numWords = vocabulary::NUM_WORDS);

    static Vocabulary* fromImageList(
        const std::string& category,
        const std::vector<boost::filesystem::path>& names,
        size_t numWords = vocabulary::NUM_WORDS);

    ~Vocabulary();

private:
    Vocabulary(const std::string category, const cv::Mat& data, size_t numWords);

private:
    std::string category;
    cv::Mat data;
    size_t numWords;
    cv::Mat words;
    boost::scoped_ptr<KDTree<float> > kdtree;
};

} /* namespace vis */

#endif /* VIL_VOCABULARY_H */

