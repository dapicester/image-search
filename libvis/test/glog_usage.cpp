/**
 * @file glog_usage.cpp
 * @brief Usage test for the GLOG library
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE glog
#include <boost/test/unit_test.hpp>

#include <glog/logging.h>

/// When logging has not been initialized, logs go to stderr.
/// NOTE: logging without initialization prints also a warning message.
BOOST_AUTO_TEST_CASE(glog_usage) {
    LOG(INFO) << "This is an INFO log message, should go to stderr";
    LOG(WARNING) << "This is a WARNING log message, should go to stderr";
    LOG(ERROR) << "This is an ERROR log message, should go to stderr";

    // NOTE: FATAL messages terminate the program with a SIGABRT after printing the message.
    //LOG(FATAL) << "This is a FATAL log message, should go to stderr and abort";

    LOG_IF(INFO, 2 + 2 == 4) << "I know math!";

    for (int i = 0; i < 10; i++) {
        LOG_EVERY_N(INFO, 2) << "counter = " << google::COUNTER;
        LOG_FIRST_N(INFO, 5) << "i = " << i;
        LOG_IF_EVERY_N(INFO, i % 2 == 0, 2) << "i is even: " << i;
    }

    DLOG(INFO) << "This message is compiled away in non-debug build";
    // NOTE: can use all macros above with D-prefix

    // The lower the verbose level, the more likely messages are to be logger (opposite of severity)
    // NOTE: run the program with GLOG_v=n environment variable set
    VLOG(1) << "I'm printed when you run the program with --v=1 or higher";
    VLOG(2) << "I'm printed when you run the program with --v=2 or higher";
    if (VLOG_IS_ON(3)) {
        // do stuff which could not be done using only VLOG(n)
        LOG(INFO) << "I'm printed when you run the program with --v=3 or higher";
    }
    // NOTE: can use all macros above with V-prefix
}

/// Checking for assertions causes a SIGABRT in case of failure.
BOOST_AUTO_TEST_CASE(glog_assertions) {
    CHECK(1+1 == 2) << "Check condition";
    CHECK_NE(1, 2) << "Check disequality";
    CHECK_EQ(1, 1) << "Check equality";

    int i = 1, *ptr = &i;
    CHECK_NOTNULL(ptr);

    const char string[] = "hello";
    CHECK_STREQ("hello", string) << "strings equality";
    CHECK_STRNE("Hello", string) << "strings disequality";
    CHECK_STRCASEEQ("Hello", string) << "string equality case insensitive";
    CHECK_STRCASENE("Hello", NULL) << "string disequality case insensitive, safe with NULL";

    CHECK_DOUBLE_EQ(1 + 1e-16, 1 + 1e-17);
    CHECK_NEAR(1.0, 1.001, 0.001);

    // these fail
    //CHECK_DOUBLE_EQ(1 + 1e-16, 1 + 1e-15);
    //CHECK_NEAR(1.0, 1.001, 0.002);
}

// Initialization fixture.
struct InitGlog {
    InitGlog() { google::InitGoogleLogging("glog_usage"); }
    ~InitGlog() { google::ShutdownGoogleLogging(); }
};

BOOST_FIXTURE_TEST_SUITE(glog_init, InitGlog)

/// Logs go by default to /tmp/${argv[0]} ...
BOOST_AUTO_TEST_CASE(defaults) {
    // NOTE: actually files glog_usage.{INFO,WARNING,ERROR} are symlinks to timestamped files
    LOG(INFO) << "This is an INFO log message, should go to files /tmp/glog_usage.{INFO,WARNING,ERROR}";
    LOG(WARNING) << "This is a WARN log message, should go to files /tmp/glog_usage.{WARNING,ERROR}";
    LOG(ERROR) << "This is an ERROR log message, should go to file /tmp/glog_usage.ERROR and to stderr";
}

/// Send logs to stderr.
BOOST_AUTO_TEST_CASE(to_stderr) {
    google::LogToStderr();

    LOG(INFO) << "This is an INFO log message, should go to stderr";
    LOG(WARNING) << "This is a WARNING log message, should go to stderr";
    LOG(ERROR) << "This is an ERROR log message, should go to stderr";
}

/// Set logs destination without using --log_dir command line argument.
BOOST_AUTO_TEST_CASE(to_destination) {
    google::SetLogDestination(google::INFO, "logfile.INFO.");

    // NOTE actually files glog_usage.{INFO,WARNING,ERROR} are symlinks to timestamped files
    LOG(INFO) << "This is an INFO log message, should go to file logfile.INFO.something";

    // these go to /tmp/ as defaults
    LOG(WARNING) << "This is a WARNING log message, should go also to file /tmp/glog_usage.{WARNING,ERROR}";
    LOG(ERROR) << "This is an ERROR log message, should go also to file /tmp/glog_usage.{ERROR} and stderr";
}

BOOST_AUTO_TEST_SUITE_END()

