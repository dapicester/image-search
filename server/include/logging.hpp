/**
 * @file logging.hpp
 * @brief Enable logging system using EasyLogging++.
 * @author Paolo D'Apice
 */

#ifndef VIS_LOGGING_HPP
#define VIS_LOGGING_HPP

#include <easylogging++.h>

namespace vis {
namespace logging {

/// @brief Initializes a named logger.
void initLogger(const std::string& loggerId = "default");

/// @brief Registers loggers.
void registerLoggers(const std::vector<std::string> names);

} // namespace logging
} // namespace vis

#endif // VIS_LOGGING_HPP

