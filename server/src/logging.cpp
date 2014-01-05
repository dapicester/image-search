/**
 * @file logging.cpp
 * @brief Enable logging system using EasyLogging++.
 * @author Paolo D'Apice
 */

#include "logging.hpp"

namespace vis {
namespace logging {

void
initLogger(const std::string& loggerId) {
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

void
registerLoggers(const std::vector<std::string> names) {
    std::for_each(names.begin(), names.end(), [](const std::string& logger) {
        initLogger(logger);
    });
}

} // namespace logging
} // namespace vis

