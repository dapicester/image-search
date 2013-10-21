#ifndef VIS_LOGGING_HPP
#define VIS_LOGGING_HPP

#include <glog/logging.h>

namespace vis {

/// @brief Initializer for the Google Logging Library.
struct LoggingInitializer {

    /// Constructor, initializes logging.
    LoggingInitializer(const char* argv0, bool toStderr = false) {
        google::InitGoogleLogging(argv0);
        if (toStderr) google::LogToStderr();
    }

    /// Destructor, shutdown logging.
    ~LoggingInitializer() {
        google::ShutdownGoogleLogging();
    }

};

} /* namespace vis */

#endif /* VIS_LOGGING_HPP */

