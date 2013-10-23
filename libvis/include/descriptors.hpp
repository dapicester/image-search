/**
 * @file descriptors.hpp
 * @brief Wrapper for descriptors.
 * @author Paolo D'Apice
 */

#ifndef VIS_DESCRIPTORS_HPP
#define VIS_DESCRIPTORS_HPP

#include "descriptors_type.hpp"
#include "extract.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

class Descriptors {
public:
    Descriptors();
    ~Descriptors();

    template <typename Callback>
    void compute(const std::string& category,
                 const std::vector<boost::filesystem::path>& files,
                 const Callback& cb,
                 LoadImage flag = vis::COLORS);

    std::string getCategory() const;

    const cv::Mat& get() const;

    static Descriptors* load(const boost::filesystem::path& file);

    void save(const boost::filesystem::path& file) const;

private:

    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version);

private:
    std::string category;
    boost::scoped_ptr<cv::Mat> descriptors;
};

inline std::string
Descriptors::getCategory() const {
    return category;
}

inline const cv::Mat&
Descriptors::get() const {
    return *descriptors;
}

template <typename Archive>
void
Descriptors::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & descriptors;
}

template <typename Callback>
void
Descriptors::compute(const std::string& category,
        const std::vector<boost::filesystem::path>& files,
        const Callback& cb, LoadImage flag) {
    this->category = category;
    this->descriptors.reset(new cv::Mat);
    vis::extract(files, *descriptors, cb, flag);
}

} /* namespace vis */

#endif /* VIS_DESCRIPTORS_HPP */

