/**
 * @file utils/handlers.hpp
 * @brief Sample handlers.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_HANDLERS_HPP
#define VIS_UTILS_HANDLERS_HPP

#include <boost/filesystem/path.hpp>
#include <functional>

namespace vis {
namespace handlers {

static const auto PrintFile =
        [](int i, const std::vector<boost::filesystem::path>& files) {
            std::cout << "  Extracting features from " << files[i]
                  << " (" << i+1 << "/" << files.size() << ")" << std::endl;
        };

} /* namespace vis::handlers */
} /* namespace vis */

#endif /* VIS_UTILS_HANDLERS_HPP */

