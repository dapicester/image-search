/**
 * @file descriptors.hpp
 * @brief Wrapper for descriptors.
 * @author Paolo D'Apice
 */

#ifndef VIS_DESCRIPTORS_HPP
#define VIS_DESCRIPTORS_HPP

#include "descriptors_type.hpp"
#include "extract.hpp"

#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace vis {

struct Callback;

/// @brief Wrapper to descriptors container.
class Descriptors {
public:
    /// Default constructor.
    Descriptors();

    /// Default destructor.
    ~Descriptors();

    /// @brief Compute descriptors using the given callback.
    void compute(const std::string& category,
                 const std::vector<boost::filesystem::path>& files,
                 const Callback& cb,
                 ColorMode mode = ColorMode::COLORS,
                 ProgressHandler handler = NULL);

    /// @return The actual category.
    std::string getCategory() const { return category; }

    /// @return A const reference to the actual descriptor matrix.
    const arma::fmat& data() const { return descriptors; }

    /// @return The actual descriptor type.
    vis::DescriptorsType getType() const { return type; }

    /// @return The path of indexed images.
    const std::vector<boost::filesystem::path>& getFiles() const { return files; }

    /// @brief Load descriptors from file.
    static Descriptors* load(const boost::filesystem::path& file);

    /// @brief Save descriptors to file.
    void save(const boost::filesystem::path& file) const;

private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version);

private:
    std::string category;
    arma::fmat descriptors;
    vis::DescriptorsType type;
    std::vector<boost::filesystem::path> files;
};

template <typename Archive>
void
Descriptors::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & descriptors;
    ar & type;
    ar & files;
}

} /* namespace vis */

#endif /* VIS_DESCRIPTORS_HPP */

