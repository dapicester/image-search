/**
 * @file logging.hpp
 * @brief Enable logging system using EasyLogging++.
 * @author Paolo D'Apice
 */

#ifndef VIS_LOGGING_HPP
#define VIS_LOGGING_HPP

#include <easylogging++.h>

namespace vis {

/// @brief Initializes a named logger.
inline void
initLogger(const std::string& loggerId = "default") {
#if 0
    el::Configurations conf;
    conf.setToDefault();
    #if 0
    conf.setGlobally(
            el::ConfigurationType::Format,
            "%datetime %level (%thread) %msg"
        );
    #endif
    el::Loggers::reconfigureLogger(loggerId, conf);
#else
    el::Loggers::getLogger(loggerId, true); // register if not exists
#endif
}

/// @brief Registers loggers.
inline void
registerLoggers(const std::vector<std::string> names) {
    std::for_each(names.begin(), names.end(), [](const std::string& logger) {
        vis::initLogger(logger);
    });
}

} // namespace vis

#endif // VIS_LOGGING_HPP

